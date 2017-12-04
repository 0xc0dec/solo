/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"

namespace solo
{
    class VulkanRenderPassConfig
    {
    public:
        VulkanRenderPassConfig();

        auto withColorAttachment(VkFormat colorFormat, VkImageLayout finalLayout) -> VulkanRenderPassConfig&;
        auto withDepthAttachment(VkFormat depthFormat) -> VulkanRenderPassConfig&;

    private:
        friend class VulkanRenderPass;

        VkDevice device = nullptr;
        vec<VkAttachmentDescription> attachments;
        vec<VkAttachmentReference> colorAttachmentRefs;
        VkAttachmentReference depthAttachmentRef;
        vec<VkClearValue> clearValues;
    };

    class VulkanRenderPass
    {
    public:
        VulkanRenderPass() {}

        VulkanRenderPass(VkDevice device, const VulkanRenderPassConfig &config);
        VulkanRenderPass(const VulkanRenderPass &other) = delete;
        VulkanRenderPass(VulkanRenderPass &&other) = default;

        ~VulkanRenderPass() {}

        auto operator=(const VulkanRenderPass &other) -> VulkanRenderPass& = delete;
        auto operator=(VulkanRenderPass &&other) -> VulkanRenderPass& = default;

        void begin(VkCommandBuffer cmdBuf, VkFramebuffer framebuffer, u32 canvasWidth, u32 canvasHeight,
            VkClearColorValue clearColor, VkClearDepthStencilValue clearDepthStencil);
        void end(VkCommandBuffer cmdBuf);

        operator VkRenderPass() { return pass; }

    private:
        VkDevice device = nullptr;
        VulkanResource<VkRenderPass> pass;
        vec<VkClearValue> clearValues;
    };
}

#endif
