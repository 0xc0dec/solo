/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"

namespace solo {
    class VulkanRenderPassConfig {
    public:
        VulkanRenderPassConfig();

        auto addColorAttachment(VkFormat colorFormat, VkImageLayout finalLayout) -> VulkanRenderPassConfig&;
        auto setDepthAttachment(VkFormat depthFormat) -> VulkanRenderPassConfig&;

    private:
        friend class VulkanRenderPass;

        vec<VkAttachmentDescription> attachments_;
        vec<VkAttachmentReference> colorAttachmentRefs_;
        VkAttachmentReference depthAttachmentRef_;
    };

    class VulkanRenderPass {
    public:
        VulkanRenderPass() = default;
        VulkanRenderPass(VkDevice device, const VulkanRenderPassConfig &config);
        VulkanRenderPass(const VulkanRenderPass &other) = delete;
        VulkanRenderPass(VulkanRenderPass &&other) = default;
        ~VulkanRenderPass() = default;

        auto clearValues() const -> const vec<VkClearValue> & { return clearValues_; }
        auto colorAttachmentCount() const -> u32 {
            return colorAttachmentCount_;
        }

        void begin(VkCommandBuffer cmdBuf, VkFramebuffer framebuffer, u32 canvasWidth, u32 canvasHeight);
        void end(VkCommandBuffer cmdBuf);

        auto operator=(const VulkanRenderPass &other) -> VulkanRenderPass & = delete;
        auto operator=(VulkanRenderPass &&other) -> VulkanRenderPass & = default;

        auto handle() const -> VkRenderPass {
            return pass_;
        }

        operator const VkRenderPass() const {
            return pass_;
        }

    private:
        VulkanResource<VkRenderPass> pass_;
        vec<VkClearValue> clearValues_;
        u32 colorAttachmentCount_ = 0;
    };
}

#endif
