/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGLMesh.h"
#include "SoloDevice.h"
#include "SoloOpenGLEffect.h"
#include <algorithm>

using namespace solo;

static auto toPrimitiveType(PrimitiveType type) -> GLenum
{
    switch (type)
    {
        case PrimitiveType::Triangles:
            return GL_TRIANGLES;
        case PrimitiveType::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case PrimitiveType::Lines:
            return GL_LINES;
        case PrimitiveType::LineStrip:
            return GL_LINE_STRIP;
        case PrimitiveType::Points:
            return GL_POINTS;
        default:
            SL_PANIC("Unknown primitive type");
            return GL_TRIANGLES;
    }
}

gl::Mesh::~Mesh()
{
    resetVertexArrayCache();
    while (!vertexBuffers.empty())
        removeVertexBuffer(0);
    while (!indexBuffers.empty())
        removePart(0);
}

auto gl::Mesh::getOrCreateVertexArray(gl::Effect *effect) -> GLuint
{
    auto &cacheEntry = vertexArrayCache[effect];
    cacheEntry.age = 0;

    auto &handle = cacheEntry.handle;
    if (handle)
        return handle;
    
    glGenVertexArrays(1, &handle);
    SL_PANIC_IF(!handle, "Failed to create vertex array");

    glBindVertexArray(handle);

    for (u32 i = 0; i < vertexBuffers.size(); i++)
    {
        const auto &bufferHandle = vertexBuffers.at(i);
        const auto &layout = layouts.at(i);
        const auto attrCount = layout.getAttributeCount();
        if (!attrCount)
            continue;

        glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);

        u32 offset = 0;
        for (u32 j = 0; j < attrCount; j++)
        {
            const auto attr = layout.getAttribute(j);
            const auto stride = layout.getSize();
            auto location = attr.location;

            if (!attr.name.empty())
            {
                const auto attrInfo = effect->getAttributeInfo(attr.name);
                location = attrInfo.location;
            }

            glVertexAttribPointer(location, attr.elementCount, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(offset));
            glEnableVertexAttribArray(location);

            offset += attr.size;
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glBindVertexArray(0);

    return handle;
}

void gl::Mesh::resetVertexArrayCache()
{
    for (auto &p: vertexArrayCache)
        glDeleteVertexArrays(1, &p.second.handle);
    vertexArrayCache.clear();
}

void gl::Mesh::flushVertexArrayCache()
{
    uset<gl::Effect*> toRemove;
    for (auto &entry: vertexArrayCache)
    {
        if (++entry.second.age >= 1000) // TODO more sophisticated way
            toRemove.insert(entry.first);
    }
    
    for (auto &key: toRemove)
        vertexArrayCache.erase(key);
}

void gl::Mesh::updateMinVertexCount()
{
    constexpr auto max = (std::numeric_limits<u32>::max)();

    minVertexCount = max;

    for (const auto &count : vertexCounts)
        minVertexCount = (std::min)(count, minVertexCount);

    if (minVertexCount == max)
        minVertexCount = 0;
}

auto gl::Mesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    return addVertexBuffer(layout, data, vertexCount, false);
}

auto gl::Mesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    return addVertexBuffer(layout, data, vertexCount, true);
}

auto gl::Mesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount, bool dynamic) -> u32
{
    GLuint handle = 0;
    glGenBuffers(1, &handle);
    SL_PANIC_IF(!handle, "Failed to create vertex buffer handle");

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, layout.getSize() * vertexCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    vertexBuffers.push_back(handle);
    layouts.push_back(layout);
    vertexCounts.push_back(vertexCount);
    vertexSizes.push_back(layout.getSize());
    
    updateMinVertexCount();
    resetVertexArrayCache();

    return static_cast<u32>(vertexBuffers.size() - 1);
}

void gl::Mesh::updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount)
{
    const auto vertexSize = vertexSizes.at(index);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers.at(index));
    glBufferSubData(GL_ARRAY_BUFFER, vertexOffset * vertexSize, vertexCount * vertexSize, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void gl::Mesh::removeVertexBuffer(u32 index)
{
    auto handle = vertexBuffers.at(index);
    glDeleteBuffers(1, &handle);

    vertexBuffers.erase(vertexBuffers.begin() + index);
    vertexCounts.erase(vertexCounts.begin() + index);
    vertexSizes.erase(vertexSizes.begin() + index);
    layouts.erase(layouts.begin() + index);

    updateMinVertexCount();
    resetVertexArrayCache();
}

auto gl::Mesh::addPart(const void *data, u32 elementCount) -> u32
{
    GLuint handle = 0;
    glGenBuffers(1, &handle);
    SL_PANIC_IF(!handle, "Failed to create index buffer handle");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * elementCount, data, GL_STATIC_DRAW); // 2 because we currently support only UNSIGNED_SHORT indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    indexBuffers.push_back(handle);
    indexElementCounts.push_back(elementCount);

    return static_cast<u32>(indexBuffers.size() - 1);
}

void gl::Mesh::removePart(u32 part)
{
    auto handle = indexBuffers.at(part);
    glDeleteBuffers(1, &handle);
    indexBuffers.erase(indexBuffers.begin() + part);
    indexElementCounts.erase(indexElementCounts.begin() + part);
}

void gl::Mesh::draw(gl::Effect *effect)
{
    const auto va = getOrCreateVertexArray(effect);
    flushVertexArrayCache();

    if (indexBuffers.empty())
    {
        glBindVertexArray(va);
        glDrawArrays(toPrimitiveType(primitiveType), 0, minVertexCount);
        glBindVertexArray(0);
    }
    else
    {
        for (auto i = 0; i < indexBuffers.size(); i++)
            drawPart(i, effect);
    }
}

void gl::Mesh::drawPart(u32 part, gl::Effect *effect)
{
    const auto va = getOrCreateVertexArray(effect);
    flushVertexArrayCache();

    glBindVertexArray(va);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers.at(part));
    glDrawElements(toPrimitiveType(primitiveType), indexElementCounts.at(part), GL_UNSIGNED_SHORT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

#endif
