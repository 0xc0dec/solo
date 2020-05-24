/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
    renderer_ = dynamic_cast<VulkanRenderer*>(device->renderer());
}

auto VulkanMesh::addVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32
{
	vertexBuffers_.push_back(VulkanBuffer::deviceLocal(renderer_->device(), layout.size() * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data.data()));
    return Mesh::addVertexBuffer(layout, data, vertexCount);
}

auto VulkanMesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32
{
	vertexBuffers_.push_back(VulkanBuffer::hostVisible(renderer_->device(), layout.size() * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data.data()));
    return Mesh::addDynamicVertexBuffer(layout, data, vertexCount);
}

void VulkanMesh::updateVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount)
{
    const auto vertexSize = layouts_[index].size();
    vertexBuffers_[index].updatePart(data, vertexOffset * vertexSize, vertexCount * vertexSize);
	Mesh::updateVertexBuffer(index, vertexOffset, data, vertexCount);
}

void VulkanMesh::removeVertexBuffer(u32 index)
{
    vertexBuffers_.erase(vertexBuffers_.begin() + index);
    Mesh::removeVertexBuffer(index);
}

auto VulkanMesh::addPart(const vec<u32> &data, u32 indexElementCount) -> u32
{
    const auto size = static_cast<VkDeviceSize>(IndexElementSize::Bits32) * indexElementCount; // TODO 16-bit support?
    auto buf = VulkanBuffer::deviceLocal(renderer_->device(), size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, data.data());
    indexBuffers_.push_back(std::move(buf));
	return Mesh::addPart(data, indexElementCount);
}

void VulkanMesh::removePart(u32 index)
{
    indexBuffers_.erase(indexBuffers_.begin() + index);
	Mesh::removePart(index);
}

auto VulkanMesh::primitiveType() const -> PrimitiveType
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

        for (u32 j = 0; j < layout.attributeCount(); j++)
        {
            const auto attr = layout.attribute(j);
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

#endif
