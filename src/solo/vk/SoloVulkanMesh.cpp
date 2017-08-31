/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanMesh.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"
#include <algorithm>

using namespace solo;

vk::Mesh::Mesh(Device *device)
{
    renderer = dynamic_cast<Renderer*>(device->getRenderer());
}

vk::Mesh::~Mesh()
{
}

auto vk::Mesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t
{
    auto buf = Buffer::createDeviceLocal(renderer, layout.getSize() * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data);

    vertexBuffers.push_back(std::move(buf));
    layouts.push_back(layout);
    vertexCounts.push_back(vertexCount);

    updateMinVertexCount();

    return static_cast<uint32_t>(vertexBuffers.size() - 1);
}

auto vk::Mesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t
{
    return 0;
}

void vk::Mesh::updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const void *data, uint32_t vertexCount)
{
}

void vk::Mesh::removeVertexBuffer(uint32_t index)
{
    vertexBuffers.erase(vertexBuffers.begin() + index);
    layouts.erase(layouts.begin() + index);
    vertexCounts.erase(vertexCounts.begin() + index);
    updateMinVertexCount();
}

auto vk::Mesh::addPart(const void *indexData, uint32_t indexElementCount) -> uint32_t
{
    auto size = sizeof(uint16_t) * indexElementCount;
    auto buf = Buffer::createDeviceLocal(renderer, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexData);
    indexBuffers.push_back(std::move(buf));
    indexElementCounts.push_back(indexElementCount);
    return static_cast<uint32_t>(indexElementCounts.size() - 1);
}

void vk::Mesh::removePart(uint32_t index)
{
    indexBuffers.erase(indexBuffers.begin() + index);
    indexElementCounts.erase(indexElementCounts.begin() + index);
}

auto vk::Mesh::getPartCount() const -> uint32_t
{
    return indexBuffers.size();
}

auto vk::Mesh::getPrimitiveType() const -> PrimitiveType
{
    return PrimitiveType::Triangles;
}

void vk::Mesh::setPrimitiveType(PrimitiveType type)
{
}

void vk::Mesh::updateMinVertexCount()
{
    constexpr auto max = (std::numeric_limits<uint32_t>::max)();

    minVertexCount = max;

    for (const auto &count : vertexCounts)
        minVertexCount = (std::min)(count, minVertexCount);

    if (minVertexCount == max)
        minVertexCount = 0;
}

#endif
