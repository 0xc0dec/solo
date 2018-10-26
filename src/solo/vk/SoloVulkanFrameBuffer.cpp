/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanFrameBuffer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"
#include "SoloVulkanTexture.h"

using namespace solo;

auto VulkanFrameBuffer::create(Device *device, const vec<sptr<Texture2D>> &attachments) -> sptr<VulkanFrameBuffer>
{
    SL_DEBUG_BLOCK(validateNewAttachments(attachments));

    const auto renderer = static_cast<VulkanRenderer*>(device->getRenderer());
    auto result = sptr<VulkanFrameBuffer>(new VulkanFrameBuffer());

    vec<VkImageView> views;
    VulkanRenderPassConfig config;

    for (const auto &tex: attachments)
    {
        const auto vkTexture = std::static_pointer_cast<VulkanTexture2D>(tex);
        if (tex->getFormat() == TextureFormat::Depth24)
            result->depthAttachment_ = vkTexture;
        else
        {
            result->colorAttachments_.push_back(vkTexture);
            config.withColorAttachment(vkTexture->image().format(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL); // TODO Proper layout
            views.push_back(vkTexture->image().view());
        }
    }

    result->dimensions_ = attachments[0]->getDimensions();

    if (!result->depthAttachment_)
    {
        result->depthAttachment_ = VulkanTexture2D::createEmpty(device,
            static_cast<u32>(result->dimensions_.x()), static_cast<u32>(result->dimensions_.y()), TextureFormat::Depth24);
    }

    views.push_back(result->depthAttachment_->image().view());
    config.withDepthAttachment(result->depthAttachment_->image().format());

    result->renderPass_ = VulkanRenderPass(renderer->device(), config);
    result->frameBuffer_ = vk::createFrameBuffer(renderer->device(), views, result->renderPass_, result->dimensions_.x(), result->dimensions_.y());

    return result;
}

#endif
