/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanDescriptorSet.h"
#include "SoloVulkanBuffer.h"
#include "SoloVulkan.h"
#include "math/SoloVector4.h"
#include "SoloVulkanImage.h"

namespace solo
{
    class VulkanRenderPass;

    class VulkanCmdBuffer
    {
    public:
        VulkanCmdBuffer() = default;
        VulkanCmdBuffer(const VulkanDriverDevice &dev);
        VulkanCmdBuffer(const VulkanCmdBuffer &other) = delete;
        VulkanCmdBuffer(VulkanCmdBuffer &&other) = default;
        ~VulkanCmdBuffer() = default;

        auto begin(bool transient) -> VulkanCmdBuffer&; // TODO better understanding of the `transient` param
        void end() const;
        void endAndFlush();

        auto beginRenderPass(const VulkanRenderPass &pass, VkFramebuffer framebuffer, u32 canvasWidth, u32 canvasHeight)
            -> VulkanCmdBuffer&;
        auto endRenderPass() -> VulkanCmdBuffer&;

        auto bindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) -> VulkanCmdBuffer&;
        auto bindVertexBuffer(u32 binding, VkBuffer buffer) -> VulkanCmdBuffer&;
        auto drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, u32 vertexOffset, u32 firstInstance)
            -> VulkanCmdBuffer&;
        auto draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) -> VulkanCmdBuffer&;

        auto bindPipeline(VkPipeline pipeline) -> VulkanCmdBuffer&;
        auto bindDescriptorSet(VkPipelineLayout pipelineLayout, const VulkanDescriptorSet &set) -> VulkanCmdBuffer&;

        auto setViewport(const Vector4 &dimentions, float minDepth, float maxDepth) -> VulkanCmdBuffer&;
        auto setScissor(const Vector4 &dimentions) -> VulkanCmdBuffer&;

        auto putImagePipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
            const VkImageMemoryBarrier &barrier) -> VulkanCmdBuffer&;

        auto clearColorAttachment(u32 attachment, const VkClearValue &clearValue, const VkClearRect &clearRect)
            -> VulkanCmdBuffer&;

        auto copyBuffer(const VulkanBuffer &src, const VulkanBuffer &dst) -> VulkanCmdBuffer&;
        auto copyBuffer(const VulkanBuffer &src, const VulkanImage &dst) -> VulkanCmdBuffer&;
        auto copyBuffer(const VulkanBuffer &src, const VulkanImage &dst, 
            const VkBufferImageCopy *regions, u32 regionCount) -> VulkanCmdBuffer&;

        auto blit(VkImage src, VkImage dst, VkImageLayout srcLayout, VkImageLayout dstLayout,
            const VkImageBlit &blit, VkFilter filter) -> VulkanCmdBuffer&;

        auto operator=(const VulkanCmdBuffer &other) -> VulkanCmdBuffer& = delete;
        auto operator=(VulkanCmdBuffer &&other) -> VulkanCmdBuffer& = default;

        operator bool() const { return handle_; }
    	operator VkCommandBuffer() const { return handle_; }
        operator const VkCommandBuffer*() { return &handle_; }

    private:
        const VulkanDriverDevice *device_ = nullptr;
        VulkanResource<VkCommandBuffer> handle_;
    };
}

#endif