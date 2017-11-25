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
    class VulkanRenderer;

    class VulkanFrameBuffer final: public FrameBuffer
    {
    public:
        VulkanFrameBuffer(VulkanRenderer *renderer);
        ~VulkanFrameBuffer();

        void setAttachments(const vec<sptr<Texture2d>> &attachments) override final;

        auto getRenderPass() -> VulkanRenderPass& { return renderPass; }

    private:
        VulkanRenderer *renderer = nullptr;
        VulkanRenderPass renderPass;
        VulkanImage depthStencil;
        VulkanResource<VkFramebuffer> frameBuffer;
    };
}

#endif
