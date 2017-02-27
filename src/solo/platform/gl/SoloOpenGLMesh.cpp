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


gl::Mesh::Mesh(Device *device)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
}


gl::Mesh::~Mesh()
{
    if (vertexArray != EmptyHandle)
        renderer->destroyVertexArray(vertexArray);
    while (!vertexBuffers.empty())
        removeVertexBuffer(0);
    while (!indexBuffers.empty())
        removePart(0);
}


auto gl::Mesh::addVertexBuffer(uint32_t bufferHandle, const VertexBufferLayout &layout, uint32_t vertexCount) -> uint32_t
{
    vertexBuffers.push_back(bufferHandle);
    vertexCounts.push_back(vertexCount);
    vertexSizes.push_back(layout.getSize());
    updateMinVertexCount();
    dirtyVertexArray = true;
    return static_cast<uint32_t>(vertexBuffers.size() - 1);
}


void gl::Mesh::rebuildVertexArray() const
{
    if (!dirtyVertexArray)
        return;
    if (vertexArray != EmptyHandle)
        renderer->destroyVertexArray(vertexArray);
    vertexArray = renderer->createVertexArray(vertexBuffers.data(), static_cast<uint32_t>(vertexBuffers.size()));
    dirtyVertexArray = false;
}


void gl::Mesh::updateMinVertexCount()
{
    constexpr auto max = (std::numeric_limits<uint32_t>::max)();

    minVertexCount = max;

    for (const auto &count : vertexCounts)
        minVertexCount = (std::min)(count, minVertexCount);

    if (minVertexCount == max)
        minVertexCount = 0;
}


auto gl::Mesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t
{
    auto handle = renderer->createVertexBuffer(layout, data, vertexCount);
    return addVertexBuffer(handle, layout, vertexCount);
}


auto gl::Mesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t
{
    auto handle = renderer->createDynamicVertexBuffer(layout, data, vertexCount);
    return addVertexBuffer(handle, layout, vertexCount);
}


void gl::Mesh::updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const void *data, uint32_t vertexCount)
{
    const auto &handle = vertexBuffers[index];
    auto vertexSize = vertexSizes[index];
    renderer->updateDynamicVertexBuffer(handle, data, vertexOffset * vertexSize, vertexCount * vertexSize);
}


void gl::Mesh::removeVertexBuffer(uint32_t index)
{
    renderer->destroyVertexBuffer(vertexBuffers[index]);
    vertexBuffers.erase(vertexBuffers.begin() + index);
    vertexCounts.erase(vertexCounts.begin() + index);
    vertexSizes.erase(vertexSizes.begin() + index);
    updateMinVertexCount();
    dirtyVertexArray = true;
}


auto gl::Mesh::addPart(const void *data, uint32_t elementCount) -> uint32_t
{
    auto handle = renderer->createIndexBuffer(data, 2, elementCount); // 2 because we currently support only UNSIGNED_SHORT indexes
    indexBuffers.push_back(handle);
    return static_cast<uint32_t>(indexBuffers.size() - 1);
}


void gl::Mesh::removePart(uint32_t part)
{
    renderer->destroyIndexBuffer(indexBuffers[part]);
    indexBuffers.erase(indexBuffers.begin() + part);
}


void gl::Mesh::draw() const
{
    rebuildVertexArray();

    if (indexBuffers.empty())
        renderer->draw(primitiveType, vertexArray, minVertexCount);
    else
    {
        for (auto i = 0; i < indexBuffers.size(); i++)
            renderer->drawIndexed(primitiveType, vertexArray, indexBuffers.at(i));
    }
}


void gl::Mesh::drawPart(uint32_t part) const
{
    rebuildVertexArray();
    if (vertexArray != EmptyHandle)
        renderer->drawIndexed(primitiveType, vertexArray, indexBuffers.at(part));
}

#endif
