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

static void validateNewAttachments(const vec<sptr<Texture2D>> &attachments)
{
    auto width = -1, height = -1;
    for (const auto &attachment : attachments)
    {
        auto size = attachment->getDimensions();
        if (width < 0)
        {
            width = size.x();
            height = size.y();
        }
        else
            // TODO Make sure this is a valid assumption. For now I'm just copying this from opengl
            panicIf(size.x() != width || size.y() != height, "Attachment sizes do not match");
    }
}

VulkanFrameBuffer::VulkanFrameBuffer(Device *device):
    renderer(static_cast<VulkanRenderer*>(device->getRenderer()))
{
}

void VulkanFrameBuffer::setAttachments(const vec<sptr<Texture2D>> &attachments)
{
    SL_DEBUG_BLOCK(validateNewAttachments(attachments));

    if (frameBuffer)
        frameBuffer.cleanRef();

    renderPass = VulkanRenderPass();
    depthStencil = VulkanImage();
    dimensions = Vector2{0, 0};
    colorAttachmentCount = 0;

    if (!attachments.empty())
    {
        vec<VkImageView> attachmentViews;
        VulkanRenderPassConfig config;
        for (const auto &tex: attachments)
        {
            const auto vulkanTexture = std::static_pointer_cast<VulkanTexture2D>(tex);
            config.withColorAttachment(vulkanTexture->getImage().getFormat(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL); // TODO Proper layout
            attachmentViews.push_back(vulkanTexture->getImage().getView());
        }

        dimensions = attachments[0]->getDimensions();

        depthStencil = VulkanImage(
            renderer,
            dimensions.x(), dimensions.y(),
            1, 1,
            renderer->getDepthFormat(),
            0,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
            VK_IMAGE_VIEW_TYPE_2D,
            VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);

        config.withDepthAttachment(renderer->getDepthFormat());
        attachmentViews.push_back(depthStencil.getView());

        renderPass = VulkanRenderPass(renderer->getDevice(), config);
        frameBuffer = vk::createFrameBuffer(renderer->getDevice(), attachmentViews, renderPass, dimensions.x(), dimensions.y());

        colorAttachmentCount = attachments.size();
    }
}

#endif
