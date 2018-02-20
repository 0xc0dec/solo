/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
        static auto create(Device *device, const vec<sptr<Texture2D>> &attachments) -> sptr<VulkanFrameBuffer>;
        
        ~VulkanFrameBuffer() = default;

        auto getHandle() const -> VkFramebuffer { return frameBuffer; }
        auto getRenderPass() -> VulkanRenderPass& { return renderPass; }
        auto getColorAttachmentCount() const -> u32 { return static_cast<u32>(colorAttachments.size()); }

    private:
        VulkanRenderPass renderPass;
        VulkanResource<VkFramebuffer> frameBuffer;
        vec<sptr<VulkanTexture2D>> colorAttachments;
        sptr<VulkanTexture2D> depthAttachment;

        VulkanFrameBuffer() = default;
    };
}

#endif
