/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGLMesh.h"
#include "SoloDevice.h"
#include "SoloOpenGLEffect.h"

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

static auto toIndexType(IndexElementSize elementSize) -> GLenum
{
	switch (elementSize)
	{
		case IndexElementSize::Bits16: return GL_UNSIGNED_SHORT;
		case IndexElementSize::Bits32: return GL_UNSIGNED_INT;
	}

	SL_DEBUG_PANIC(true, "Unsupported index element size");
    return 0;
}

OpenGLMesh::~OpenGLMesh()
{
    resetVertexArrayCache();
    while (!vertexBuffers_.empty())
        removeVertexBuffer(0);
    while (!indexBuffers_.empty())
        removeIndexBuffer(0);
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

auto OpenGLMesh::addVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32
{
	addVertexBuffer(layout, data.data(), vertexCount, false);
	return Mesh::addVertexBuffer(layout, data, vertexCount);
}

auto OpenGLMesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32
{
	addVertexBuffer(layout, data.data(), vertexCount, true);
	return Mesh::addVertexBuffer(layout, data, vertexCount);
}

void OpenGLMesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount, bool dynamic)
{
    GLuint handle = 0;
    glGenBuffers(1, &handle);
    SL_DEBUG_PANIC(!handle, "Unable to create vertex buffer handle");

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, layout.size() * vertexCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    vertexBuffers_.push_back(handle);
    
    resetVertexArrayCache();
}

void OpenGLMesh::updateVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount)
{
	const auto vertexSize = layouts_.at(index).size();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers_.at(index));
	glBufferSubData(GL_ARRAY_BUFFER, vertexOffset * vertexSize, vertexCount * vertexSize, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	Mesh::updateVertexBuffer(index, vertexOffset, data, vertexCount);
}

void OpenGLMesh::removeVertexBuffer(u32 index)
{
	auto handle = vertexBuffers_.at(index);
	glDeleteBuffers(1, &handle);
	vertexBuffers_.erase(vertexBuffers_.begin() + index);
	resetVertexArrayCache();
	Mesh::removeVertexBuffer(index);
}

auto OpenGLMesh::addIndexBuffer(const vec<u32> &data, u32 elementCount) -> u32
{
	GLuint handle = 0;
	glGenBuffers(1, &handle);
	SL_DEBUG_PANIC(!handle, "Unable to create index buffer handle");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(IndexElementSize::Bits32) * elementCount, data.data(), GL_STATIC_DRAW); // TODO 16-bit support?
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	indexBuffers_.push_back(handle);

	return Mesh::addIndexBuffer(data, elementCount);
}

void OpenGLMesh::removeIndexBuffer(u32 index)
{
	auto handle = indexBuffers_.at(index);
	glDeleteBuffers(1, &handle);
	indexBuffers_.erase(indexBuffers_.begin() + index);
	Mesh::removeIndexBuffer(index);
}

void OpenGLMesh::render(OpenGLEffect *effect)
{
	const auto va = getOrCreateVertexArray(effect);
	flushVertexArrayCache();
	glBindVertexArray(va);
	glDrawArrays(toPrimitiveType(primitiveType_), 0, minVertexCount_);
	glBindVertexArray(0);
}

void OpenGLMesh::renderIndex(u32 index, OpenGLEffect *effect)
{
	const auto va = getOrCreateVertexArray(effect);
	flushVertexArrayCache();
	glBindVertexArray(va);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers_.at(index));
	glDrawElements(toPrimitiveType(primitiveType_), indexElementCounts_.at(index), toIndexType(IndexElementSize::Bits32), nullptr); // TODO 16-bit support?
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

#endif
