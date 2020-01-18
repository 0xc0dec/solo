/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanMesh.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloHash.h"
#include "SoloVulkanRenderer.h"
#include "SoloVulkanEffect.h"
#include <algorithm>

using namespace solo;

static auto toVertexFormat(const VertexAttribute &attr) -> VkFormat
{
    switch (attr.elementCount)
    {
        case 1: return VK_FORMAT_R32_SFLOAT;
        case 2: return VK_FORMAT_R32G32_SFLOAT;
        case 3: return VK_FORMAT_R32G32B32_SFLOAT;
        case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:
            SL_DEBUG_PANIC(true, "Unsupported vertex attribute element count");
            return VK_FORMAT_UNDEFINED;
    }
}

VulkanMesh::VulkanMesh(Device *device)
{
    renderer_ = dynamic_cast<VulkanRenderer*>(device->renderer());
}

auto VulkanMesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    auto buf = VulkanBuffer::deviceLocal(renderer_->device(), layout.size() * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data);
    return addVertexBuffer(buf, layout, vertexCount);
}

auto VulkanMesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32
{
    auto buf = VulkanBuffer::hostVisible(renderer_->device(), layout.size() * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data);
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
    const auto vertexSize = layouts_[index].size();
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
    const auto size = sizeof(u32) * indexElementCount; // TODO 16-bit index support?
    auto buf = VulkanBuffer::deviceLocal(renderer_->device(), size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexData);
    indexBuffers_.push_back(std::move(buf));
    indexElementCounts_.push_back(indexElementCount);
    return static_cast<u32>(indexElementCounts_.size() - 1);
}

void VulkanMesh::removePart(u32 index)
{
    indexBuffers_.erase(indexBuffers_.begin() + index);
    indexElementCounts_.erase(indexElementCounts_.begin() + index);
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

void VulkanMesh::updateMinVertexCount()
{
    constexpr auto max = (std::numeric_limits<u32>::max)();

    minVertexCount_ = max;

    for (const auto &count : vertexCounts_)
        minVertexCount_ = (std::min)(count, minVertexCount_);

    if (minVertexCount_ == max)
        minVertexCount_ = 0;
}

void VulkanMesh::configurePipeline(VulkanPipelineConfig &cfg, VulkanEffect *effect)
{
    const auto &effectVertexAttrs = effect->vertexAttributes();

    for (u32 binding = 0; binding < vertexBufferCount(); binding++)
    {
        const auto &layout = vertexBufferLayout(binding);
        
        cfg.withVertexBinding(binding, layout.size(), VK_VERTEX_INPUT_RATE_VERTEX);

        u32 offset = 0;
        for (u32 attrIndex = 0; attrIndex < layout.attributeCount(); attrIndex++)
        {
            const auto attr = layout.attribute(attrIndex);
            const auto format = toVertexFormat(attr);
            
            auto location = attr.location;
            auto found = true;
            if (!attr.name.empty())
            {
                if (effectVertexAttrs.count(attr.name))
                    location = effectVertexAttrs.at(attr.name).location;
                else
                    found = false;
            }
            if (found)
                cfg.withVertexAttribute(location, binding, format, offset);

            offset += attr.size;
        }
    }
}

#endif
