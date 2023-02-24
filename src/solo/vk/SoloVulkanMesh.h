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

    class VulkanMesh final: public Mesh
    {
    public:
        explicit VulkanMesh(Device *device);
        ~VulkanMesh() = default;

        auto addVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32 override;
        auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32 override;
        void updateVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount) override;
        void removeVertexBuffer(u32 index) override;
        auto vertexBuffer(u32 index) const -> VkBuffer
        {
            return vertexBuffers_.at(index).handle();
        }

        auto addIndexBuffer(const vec<u32> &data, u32 elementCount) -> u32 override;
        void removeIndexBuffer(u32 index) override;
        auto indexBuffer(u32 index) const -> VkBuffer
        {
            return indexBuffers_.at(index).handle();
        }

        auto minVertexCount() const -> u32
        {
            return minVertexCount_;
        }

        auto layoutHash() const -> size_t;

    private:
        VulkanRenderer *renderer_ = nullptr;

        vec<VulkanBuffer> vertexBuffers_;
        vec<VulkanBuffer> indexBuffers_;
    };
}

#endif