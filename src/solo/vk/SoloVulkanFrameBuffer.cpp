/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanFrameBuffer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"
#include "SoloVulkanTexture.h"

using namespace solo;

auto VulkanFrameBuffer::fromAttachments(Device *device, const vec<sptr<Texture2D>> &attachments) -> sptr<VulkanFrameBuffer>
{
	asrt([&attachments]() { validateNewAttachments(attachments); });

    const auto renderer = static_cast<VulkanRenderer*>(device->renderer());
    auto result = sptr<VulkanFrameBuffer>(new VulkanFrameBuffer());

    vec<VkImageView> views;
    VulkanRenderPassConfig config;

    for (const auto &attachment: attachments)
    {
        const auto texture = std::static_pointer_cast<VulkanTexture2D>(attachment);
        if (attachment->format() == TextureFormat::Depth24)
            result->depthAttachment_ = texture;
        else
        {
            result->colorAttachments_.push_back(texture);
            config.addColorAttachment(texture->image().format(), VK_IMAGE_LAYOUT_GENERAL); // TODO better layout
            views.push_back(texture->image().view());
        }
    }

    result->dimensions_ = attachments[0]->dimensions();

    if (!result->depthAttachment_)
    {
        result->depthAttachment_ = VulkanTexture2D::empty(device,
            static_cast<u32>(result->dimensions_.x()), static_cast<u32>(result->dimensions_.y()), TextureFormat::Depth24);
    }

    views.push_back(result->depthAttachment_->image().view());
    config.setDepthAttachment(result->depthAttachment_->image().format());

    result->renderPass_ = VulkanRenderPass(renderer->device(), config);
    result->frameBuffer_ = vk::createFrameBuffer(renderer->device(), views, result->renderPass_, result->dimensions_.x(), result->dimensions_.y());

    return result;
}

#endif
