/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloFrameBuffer.h"
#include "SoloVulkanRenderPass.h"
#include "SoloVulkanImage.h"

namespace solo
{
    class VulkanTexture2D;

    class VulkanFrameBuffer final: public FrameBuffer
    {
    public:
        static auto fromAttachments(Device *device, const vec<sptr<Texture2D>> &attachments) -> sptr<VulkanFrameBuffer>;

        ~VulkanFrameBuffer() = default;

        auto handle() const -> VkFramebuffer
        {
            return frameBuffer_;
        }
        auto renderPass() -> VulkanRenderPass & { return renderPass_; }
        auto colorAttachmentCount() const -> u32
        {
            return static_cast<u32>(colorAttachments_.size());
        }

    private:
        VulkanRenderPass renderPass_;
        VulkanResource<VkFramebuffer> frameBuffer_;
        vec<sptr<VulkanTexture2D>> colorAttachments_;
        sptr<VulkanTexture2D> depthAttachment_;

        VulkanFrameBuffer() = default;
    };
}

#endif
