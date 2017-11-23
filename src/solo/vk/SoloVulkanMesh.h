/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
        ~VulkanMesh();

        auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override final;
        auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override final;
        void updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount) override final;
        void removeVertexBuffer(u32 index) override final;

        auto addPart(const void *indexData, u32 indexElementCount) -> u32 override final;
        void removePart(u32 index) override final;
        auto getPartCount() const -> u32 override final;

        auto getPrimitiveType() const -> PrimitiveType override final;
        void setPrimitiveType(PrimitiveType type) override final;

        auto getVertexBufferCount() const -> u32 { return vertexBuffers.size(); }
        auto getVertexBufferLayout(u32 index) const -> VertexBufferLayout { return layouts.at(index); }
        auto getVertexBuffer(u32 index) const -> VkBuffer { return vertexBuffers.at(index).getHandle(); }
        auto getPartBuffer(u32 index) const -> VkBuffer { return indexBuffers.at(index).getHandle(); }
        auto getPartIndexElementCount(u32 index) const -> u32 { return indexElementCounts.at(index); }
        auto getMinVertexCount() const -> u32 { return minVertexCount; }

    private:
        VulkanRenderer *renderer = nullptr;

        vec<VulkanBuffer> vertexBuffers;
        vec<VulkanBuffer> indexBuffers;
        vec<VertexBufferLayout> layouts;
        vec<u32> vertexCounts;
        vec<u32> indexElementCounts;
        u32 minVertexCount = 0;

        void updateMinVertexCount();
    };
}

#endif