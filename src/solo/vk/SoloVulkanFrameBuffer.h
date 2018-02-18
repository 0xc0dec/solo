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
    class VulkanFrameBuffer final: public FrameBuffer
    {
    public:
		static auto create(Device *device, const vec<sptr<Texture2D>> &attachments) -> sptr<VulkanFrameBuffer>;
        
		~VulkanFrameBuffer() = default;

        auto getHandle() const -> VkFramebuffer { return frameBuffer; }
        auto getRenderPass() -> VulkanRenderPass& { return renderPass; }
        auto getColorAttachmentCount() const -> u32 { return colorAttachmentCount; }

    private:
        VulkanRenderPass renderPass;
        VulkanImage depthStencil;
        VulkanResource<VkFramebuffer> frameBuffer;
        u32 colorAttachmentCount = 0;

		VulkanFrameBuffer() = default;
    };
}

#endif
