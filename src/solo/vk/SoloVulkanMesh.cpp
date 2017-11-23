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

vk::VulkanMesh::VulkanMesh(Device *device)
{
    renderer = dynamic_cast<VulkanRenderer*>(device->getRenderer());
}

vk::VulkanMesh::~VulkanMesh()
{
}

auto vk::VulkanMesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    auto buf = VulkanBuffer::createDeviceLocal(renderer, layout.getSize() * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data);

    vertexBuffers.push_back(std::move(buf));
    layouts.push_back(layout);
    vertexCounts.push_back(vertexCount);

    updateMinVertexCount();

    return static_cast<u32>(vertexBuffers.size() - 1);
}

auto vk::VulkanMesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    // TODO
    return addVertexBuffer(layout, data, vertexCount);
}

void vk::VulkanMesh::updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount)
{
    // TODO
}

void vk::VulkanMesh::removeVertexBuffer(u32 index)
{
    vertexBuffers.erase(vertexBuffers.begin() + index);
    layouts.erase(layouts.begin() + index);
    vertexCounts.erase(vertexCounts.begin() + index);
    updateMinVertexCount();
}

auto vk::VulkanMesh::addPart(const void *indexData, u32 indexElementCount) -> u32
{
    const auto size = sizeof(uint16_t) * indexElementCount;
    auto buf = VulkanBuffer::createDeviceLocal(renderer, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexData);
    indexBuffers.push_back(std::move(buf));
    indexElementCounts.push_back(indexElementCount);
    return static_cast<u32>(indexElementCounts.size() - 1);
}

void vk::VulkanMesh::removePart(u32 index)
{
    indexBuffers.erase(indexBuffers.begin() + index);
    indexElementCounts.erase(indexElementCounts.begin() + index);
}

auto vk::VulkanMesh::getPartCount() const -> u32
{
    return indexBuffers.size();
}

auto vk::VulkanMesh::getPrimitiveType() const -> PrimitiveType
{
    return PrimitiveType::Triangles;
}

void vk::VulkanMesh::setPrimitiveType(PrimitiveType type)
{
}

void vk::VulkanMesh::updateMinVertexCount()
{
    constexpr auto max = (std::numeric_limits<u32>::max)();

    minVertexCount = max;

    for (const auto &count : vertexCounts)
        minVertexCount = (std::min)(count, minVertexCount);

    if (minVertexCount == max)
        minVertexCount = 0;
}

#endif
