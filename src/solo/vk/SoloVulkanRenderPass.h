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
    namespace vk
    {
        class RenderPassConfig
        {
        public:
            RenderPassConfig();

            auto withColorAttachment(VkFormat colorFormat, VkImageLayout finalLayout, bool clear, VkClearColorValue clearValue = {}) -> RenderPassConfig&;
            auto withDepthAttachment(VkFormat depthFormat, bool clear, VkClearDepthStencilValue clearValue = {}) -> RenderPassConfig&;

        private:
            friend class RenderPass;

            VkDevice device = nullptr;
            vec<VkAttachmentDescription> attachments;
            vec<VkAttachmentReference> colorAttachmentRefs;
            VkAttachmentReference depthAttachmentRef;
            vec<VkClearValue> clearValues;
        };

        class RenderPass
        {
        public:
            RenderPass() {}

            RenderPass(VkDevice device, const RenderPassConfig &config);
            RenderPass(const RenderPass &other) = delete;
            RenderPass(RenderPass &&other) = default;

            ~RenderPass() {}

            auto operator=(const RenderPass &other) -> RenderPass& = delete;
            auto operator=(RenderPass &&other) -> RenderPass& = default;

            void begin(VkCommandBuffer cmdBuf, VkFramebuffer framebuffer, u32 canvasWidth, u32 canvasHeight);
            void end(VkCommandBuffer cmdBuf);

            operator VkRenderPass() { return pass; }

        private:
            VkDevice device = nullptr;
            Resource<VkRenderPass> pass;
            vec<VkClearValue> clearValues;
        };
    }
}

#endif
