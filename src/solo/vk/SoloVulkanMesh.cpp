/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanMesh.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloHash.h"
#include "SoloVulkanRenderer.h"
#include <algorithm>

using namespace solo;

VulkanMesh::VulkanMesh(Device *device)
{
    renderer = dynamic_cast<VulkanRenderer*>(device->getRenderer());
}

VulkanMesh::~VulkanMesh()
{
}

auto VulkanMesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    auto buf = VulkanBuffer::createDeviceLocal(renderer, layout.getSize() * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data);

    vertexBuffers.push_back(std::move(buf));
    layouts.push_back(layout);
    vertexCounts.push_back(vertexCount);

    updateMinVertexCount();

    return static_cast<u32>(vertexBuffers.size() - 1);
}

// TODO less copy-paste
auto VulkanMesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    auto buf = VulkanBuffer::createHostVisible(renderer, layout.getSize() * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data);

    vertexBuffers.push_back(std::move(buf));
    layouts.push_back(layout);
    vertexCounts.push_back(vertexCount);

    updateMinVertexCount();

    return static_cast<u32>(vertexBuffers.size() - 1);
}

void VulkanMesh::updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount)
{
    const auto vertexSize = layouts[index].getSize();
    vertexBuffers[index].updatePart(data, vertexOffset * vertexSize, vertexCount * vertexSize);
}

void VulkanMesh::removeVertexBuffer(u32 index)
{
    vertexBuffers.erase(vertexBuffers.begin() + index);
    layouts.erase(layouts.begin() + index);
    vertexCounts.erase(vertexCounts.begin() + index);
    updateMinVertexCount();
}

auto VulkanMesh::addPart(const void *indexData, u32 indexElementCount) -> u32
{
    const auto size = sizeof(uint16_t) * indexElementCount;
    auto buf = VulkanBuffer::createDeviceLocal(renderer, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexData);
    indexBuffers.push_back(std::move(buf));
    indexElementCounts.push_back(indexElementCount);
    return static_cast<u32>(indexElementCounts.size() - 1);
}

void VulkanMesh::removePart(u32 index)
{
    indexBuffers.erase(indexBuffers.begin() + index);
    indexElementCounts.erase(indexElementCounts.begin() + index);
}

auto VulkanMesh::getPrimitiveType() const -> PrimitiveType
{
    // TODO
    return PrimitiveType::Triangles;
}

void VulkanMesh::setPrimitiveType(PrimitiveType type)
{
    // TODO
}

auto VulkanMesh::getLayoutHash() const -> size_t
{
    size_t seed = 0;
    const std::hash<u32> unsignedHasher;
    const std::hash<str> strHasher;

    for (s32 i = 0; i < getVertexBufferCount(); i++)
    {
        auto layout = getVertexBufferLayout(i);
        combineHash(seed, unsignedHasher(i));

        for (s32 j = 0; j < layout.getAttributeCount(); j++)
        {
            const auto attr = layout.getAttribute(j);
            combineHash(seed, unsignedHasher(j));
            combineHash(seed, strHasher(attr.name));
            combineHash(seed, unsignedHasher(attr.elementCount));
            combineHash(seed, unsignedHasher(attr.location));
            combineHash(seed, unsignedHasher(attr.offset));
            combineHash(seed, unsignedHasher(attr.size));
        }
    }

    return seed;
}

void VulkanMesh::updateMinVertexCount()
{
    constexpr auto max = (std::numeric_limits<u32>::max)();

    minVertexCount = max;

    for (const auto &count : vertexCounts)
        minVertexCount = (std::min)(count, minVertexCount);

    if (minVertexCount == max)
        minVertexCount = 0;
}

#endif
