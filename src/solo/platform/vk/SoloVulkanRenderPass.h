/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"
#include <vector>

namespace solo
{
    namespace vk
    {
        class RenderPass
        {
        public:
            RenderPass();
            RenderPass(VkDevice device, Resource<VkRenderPass> pass);
            RenderPass(const RenderPass &other) = delete;
            RenderPass(RenderPass &&other) noexcept;
            ~RenderPass() {}

            auto operator=(RenderPass other) noexcept -> RenderPass&;

            void setViewport(const VkViewport &viewport);
            void setScissor(uint32_t left, uint32_t top, uint32_t width, uint32_t height);
            void setClear(bool clearColor, bool clearDepthStencil, VkClearColorValue color, VkClearDepthStencilValue depthStencil);

            void begin(VkCommandBuffer cmdBuf, VkFramebuffer framebuffer, uint32_t canvasWidth, uint32_t canvasHeight);
            void end(VkCommandBuffer cmdBuf);

            operator VkRenderPass()
            {
                return pass;
            }

        private:
            VkDevice device = nullptr;
            Resource<VkRenderPass> pass;
            VkViewport viewport;
            VkRect2D scissor;
            std::vector<VkClearValue> clearValues;

            void swap(RenderPass &other) noexcept;
        };

        class RenderPassBuilder
        {
        public:
            explicit RenderPassBuilder(VkDevice device);

            auto withColorAttachment(VkFormat colorFormat) -> RenderPassBuilder&;
            auto withDepthAttachment(VkFormat depthFormat) -> RenderPassBuilder&;

            auto build()->RenderPass;

        private:
            VkDevice device = nullptr;
            std::vector<VkAttachmentDescription> attachments;
            std::vector<VkAttachmentReference> colorAttachmentRefs;
            VkAttachmentReference depthAttachmentRef;
        };
    }
}

#endif