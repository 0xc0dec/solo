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

        vec<VkAttachmentDescription> attachments;
        vec<VkAttachmentReference> colorAttachmentRefs;
        VkAttachmentReference depthAttachmentRef;
    };

    class VulkanRenderPass
    {
    public:
        VulkanRenderPass() = default;
        VulkanRenderPass(VkDevice device, const VulkanRenderPassConfig &config);
        VulkanRenderPass(const VulkanRenderPass &other) = delete;
        VulkanRenderPass(VulkanRenderPass &&other) = default;
        ~VulkanRenderPass() = default;

        auto operator=(const VulkanRenderPass &other) -> VulkanRenderPass& = delete;
        auto operator=(VulkanRenderPass &&other) -> VulkanRenderPass& = default;

        void begin(VkCommandBuffer cmdBuf, VkFramebuffer framebuffer, u32 canvasWidth, u32 canvasHeight);
        void end(VkCommandBuffer cmdBuf);

        operator VkRenderPass() { return pass; }

    private:
        VkDevice device = nullptr;
        VulkanResource<VkRenderPass> pass;
        vec<VkClearValue> clearValues;
    };
}

#endif
