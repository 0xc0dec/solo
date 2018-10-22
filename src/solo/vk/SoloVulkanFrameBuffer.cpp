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
            result->depthAttachment = vkTexture;
        else
        {
            result->colorAttachments.push_back(vkTexture);
            config.withColorAttachment(vkTexture->getImage().getFormat(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL); // TODO Proper layout
            views.push_back(vkTexture->getImage().getView());
        }
    }

    result->dimensions = attachments[0]->getDimensions();

    if (!result->depthAttachment)
    {
        result->depthAttachment = VulkanTexture2D::createEmpty(device,
            static_cast<u32>(result->dimensions.x()), static_cast<u32>(result->dimensions.y()), TextureFormat::Depth24);
    }

    views.push_back(result->depthAttachment->getImage().getView());
    config.withDepthAttachment(result->depthAttachment->getImage().getFormat());

    result->renderPass = VulkanRenderPass(renderer->device(), config);
    result->frameBuffer = vk::createFrameBuffer(renderer->device(), views, result->renderPass, result->dimensions.x(), result->dimensions.y());

    return result;
}

#endif
