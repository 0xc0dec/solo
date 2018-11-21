/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
    }

    SL_DEBUG_PANIC(true, "Unsupported primitive type");
    return 0;
}

OpenGLMesh::~OpenGLMesh()
{
    resetVertexArrayCache();
    while (!vertexBuffers_.empty())
        removeVertexBuffer(0);
    while (!indexBuffers_.empty())
        removePart(0);
}

auto OpenGLMesh::getOrCreateVertexArray(OpenGLEffect *effect) -> GLuint
{
    auto &cacheEntry = vertexArrayCache_[effect];
    cacheEntry.age = 0;

    auto &handle = cacheEntry.handle;
    if (handle)
        return handle;
    
    glGenVertexArrays(1, &handle);
    SL_DEBUG_PANIC(!handle, "Unable to create vertex array");

    glBindVertexArray(handle);

    for (u32 i = 0; i < vertexBuffers_.size(); i++)
    {
        const auto &bufferHandle = vertexBuffers_.at(i);
        const auto &layout = layouts_.at(i);
        const auto attrCount = layout.attributeCount();
        if (!attrCount)
            continue;

        glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);

        u32 offset = 0;
        for (u32 j = 0; j < attrCount; j++)
        {
            const auto attr = layout.attribute(j);
            const auto stride = layout.size();
            
            auto location = attr.location;
            auto found = true;
            if (!attr.name.empty())
            {
                if (effect->hasAttribute(attr.name))
                {
                    const auto attrInfo = effect->attributeInfo(attr.name);
                    location = attrInfo.location;
                }
                else
                    found = false;
            }

            if (found)
            {
                glVertexAttribPointer(location, attr.elementCount, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(offset));
                glEnableVertexAttribArray(location);
            }

            offset += attr.size;
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glBindVertexArray(0);

    return handle;
}

void OpenGLMesh::resetVertexArrayCache()
{
    for (auto &p: vertexArrayCache_)
        glDeleteVertexArrays(1, &p.second.handle);
    vertexArrayCache_.clear();
}

void OpenGLMesh::flushVertexArrayCache()
{
    uset<OpenGLEffect*> toRemove;
    for (auto &entry: vertexArrayCache_)
    {
        if (++entry.second.age >= 1000) // TODO more sophisticated way
            toRemove.insert(entry.first);
    }
    
    for (auto &key: toRemove)
        vertexArrayCache_.erase(key);
}

void OpenGLMesh::updateMinVertexCount()
{
    constexpr auto max = (std::numeric_limits<u32>::max)();

    minVertexCount_ = max;

    for (const auto &count : vertexCounts_)
        minVertexCount_ = (std::min)(count, minVertexCount_);

    if (minVertexCount_ == max)
        minVertexCount_ = 0;
}

auto OpenGLMesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    return addVertexBuffer(layout, data, vertexCount, false);
}

auto OpenGLMesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    return addVertexBuffer(layout, data, vertexCount, true);
}

auto OpenGLMesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount, bool dynamic) -> u32
{
    GLuint handle = 0;
    glGenBuffers(1, &handle);
    SL_DEBUG_PANIC(!handle, "Unable to create vertex buffer handle");

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, layout.size() * vertexCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    vertexBuffers_.push_back(handle);
    layouts_.push_back(layout);
    vertexCounts_.push_back(vertexCount);
    vertexSizes_.push_back(layout.size());
    
    updateMinVertexCount();
    resetVertexArrayCache();

    return static_cast<u32>(vertexBuffers_.size() - 1);
}

void OpenGLMesh::updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount)
{
    const auto vertexSize = vertexSizes_.at(index);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers_.at(index));
    glBufferSubData(GL_ARRAY_BUFFER, vertexOffset * vertexSize, vertexCount * vertexSize, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLMesh::removeVertexBuffer(u32 index)
{
    auto handle = vertexBuffers_.at(index);
    glDeleteBuffers(1, &handle);

    vertexBuffers_.erase(vertexBuffers_.begin() + index);
    vertexCounts_.erase(vertexCounts_.begin() + index);
    vertexSizes_.erase(vertexSizes_.begin() + index);
    layouts_.erase(layouts_.begin() + index);

    updateMinVertexCount();
    resetVertexArrayCache();
}

auto OpenGLMesh::addPart(const void *data, u32 elementCount) -> u32
{
    GLuint handle = 0;
    glGenBuffers(1, &handle);
    SL_DEBUG_PANIC(!handle, "Unable to create index buffer handle");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * elementCount, data, GL_STATIC_DRAW); // TODO support for 16-bit indices?
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    indexBuffers_.push_back(handle);
    indexElementCounts_.push_back(elementCount);

    return static_cast<u32>(indexBuffers_.size() - 1);
}

void OpenGLMesh::removePart(u32 part)
{
    auto handle = indexBuffers_.at(part);
    glDeleteBuffers(1, &handle);
    indexBuffers_.erase(indexBuffers_.begin() + part);
    indexElementCounts_.erase(indexElementCounts_.begin() + part);
}

void OpenGLMesh::draw(OpenGLEffect *effect)
{
    const auto va = getOrCreateVertexArray(effect);
    flushVertexArrayCache();

    if (indexBuffers_.empty())
    {
        glBindVertexArray(va);
        glDrawArrays(toPrimitiveType(primitiveType_), 0, minVertexCount_);
        glBindVertexArray(0);
    }
    else
    {
        for (auto i = 0; i < indexBuffers_.size(); i++)
            drawPart(i, effect);
    }
}

void OpenGLMesh::drawPart(u32 part, OpenGLEffect *effect)
{
    const auto va = getOrCreateVertexArray(effect);
    flushVertexArrayCache();

    glBindVertexArray(va);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers_.at(part));
    glDrawElements(toPrimitiveType(primitiveType_), indexElementCounts_.at(part), GL_UNSIGNED_INT, nullptr); // TODO support for 16-bit indices?
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

#endif
