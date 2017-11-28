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
        VulkanFrameBuffer(Device *device);
        ~VulkanFrameBuffer();

        void setAttachments(const vec<sptr<Texture2d>> &attachments) override final;

        auto getHandle() const -> VkFramebuffer { return frameBuffer; }
        auto getRenderPass() -> VulkanRenderPass& { return renderPass; }

    private:
        VulkanRenderer *renderer = nullptr;
        VulkanRenderPass renderPass;
        VulkanImage depthStencil;
        VulkanResource<VkFramebuffer> frameBuffer;
    };
}

#endif
