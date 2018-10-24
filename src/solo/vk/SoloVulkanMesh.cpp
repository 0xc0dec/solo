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
    renderer_ = dynamic_cast<VulkanRenderer*>(device->getRenderer());
}

auto VulkanMesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    auto buf = VulkanBuffer::createDeviceLocal(renderer_, layout.getSize() * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data);
    return addVertexBuffer(buf, layout, vertexCount);
}

auto VulkanMesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    auto buf = VulkanBuffer::createHostVisible(renderer_, layout.getSize() * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data);
    return addVertexBuffer(buf, layout, vertexCount);
}

auto VulkanMesh::addVertexBuffer(VulkanBuffer &buffer, const VertexBufferLayout &layout, u32 vertexCount) -> s32
{
    vertexBuffers_.push_back(std::move(buffer));
    layouts_.push_back(layout);
    vertexCounts_.push_back(vertexCount);

    updateMinVertexCount();

    return static_cast<u32>(vertexBuffers_.size() - 1);
}

void VulkanMesh::updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount)
{
    const auto vertexSize = layouts_[index].getSize();
    vertexBuffers_[index].updatePart(data, vertexOffset * vertexSize, vertexCount * vertexSize);
}

void VulkanMesh::removeVertexBuffer(u32 index)
{
    vertexBuffers_.erase(vertexBuffers_.begin() + index);
    layouts_.erase(layouts_.begin() + index);
    vertexCounts_.erase(vertexCounts_.begin() + index);
    updateMinVertexCount();
}

auto VulkanMesh::addPart(const void *indexData, u32 indexElementCount) -> u32
{
    const auto size = sizeof(u16) * indexElementCount;
    auto buf = VulkanBuffer::createDeviceLocal(renderer_, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexData);
    indexBuffers_.push_back(std::move(buf));
    indexElementCounts_.push_back(indexElementCount);
    return static_cast<u32>(indexElementCounts_.size() - 1);
}

void VulkanMesh::removePart(u32 index)
{
    indexBuffers_.erase(indexBuffers_.begin() + index);
    indexElementCounts_.erase(indexElementCounts_.begin() + index);
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

auto VulkanMesh::layoutHash() const -> size_t
{
    size_t seed = 0;
    const std::hash<u32> unsignedHasher;
    const std::hash<str> strHasher;

    for (u32 i = 0; i < vertexBufferCount(); i++)
    {
        auto layout = vertexBufferLayout(i);
        combineHash(seed, unsignedHasher(i));

        for (u32 j = 0; j < layout.getAttributeCount(); j++)
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

    minVertexCount_ = max;

    for (const auto &count : vertexCounts_)
        minVertexCount_ = (std::min)(count, minVertexCount_);

    if (minVertexCount_ == max)
        minVertexCount_ = 0;
}

#endif
