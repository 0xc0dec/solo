/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloMesh.h"
#include "SoloVulkanBuffer.h"
#include "SoloVulkanPipeline.h"

namespace solo
{
    class VulkanRenderer;
    class VulkanPipelineConfig;
    class VulkanEffect;

    class VulkanMesh final: public Mesh
    {
    public:
        explicit VulkanMesh(Device *device);
        ~VulkanMesh() = default;

        auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override;
        auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override;
        void updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount) override;
        void removeVertexBuffer(u32 index) override;

        auto addPart(const void *indexData, u32 indexElementCount, IndexElementSize elementSize) -> u32 override;
        void removePart(u32 index) override;

        auto primitiveType() const -> PrimitiveType override;
        void setPrimitiveType(PrimitiveType type) override;

        auto vertexBufferCount() const -> u32 { return static_cast<u32>(vertexBuffers_.size()); }
        auto vertexBufferLayout(u32 index) const -> VertexBufferLayout { return layouts_.at(index); }
        auto vertexBuffer(u32 index) const -> VkBuffer { return vertexBuffers_.at(index).handle(); }
        auto partBuffer(u32 index) const -> VkBuffer { return indexBuffers_.at(index).handle(); }
        auto partIndexElementCount(u32 index) const -> u32 { return indexElementCounts_.at(index); }
        auto minVertexCount() const -> u32 { return minVertexCount_; }

        auto layoutHash() const -> size_t;

    private:
        VulkanRenderer *renderer_ = nullptr;

        vec<VulkanBuffer> vertexBuffers_;
        vec<VulkanBuffer> indexBuffers_;
    };
}

#endif