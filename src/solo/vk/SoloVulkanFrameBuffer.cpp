/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanFrameBuffer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanRenderer.h"
#include "SoloVulkanTexture.h"

using namespace solo;

static void validateNewAttachments(const vec<sptr<Texture2d>> &attachments)
{
    auto width = -1, height = -1;
    for (auto i = 0; i < attachments.size(); i++)
    {
        auto size = attachments.at(i)->getDimensions();
        if (width < 0)
        {
            width = size.x;
            height = size.y;
        }
        else
            // TODO Make sure this is a valid assumption. For now I'm just copying this from opengl
            SL_PANIC_IF(size.x != width || size.y != height, "Attachment sizes do not match");
    }
}

VulkanFrameBuffer::VulkanFrameBuffer(VulkanRenderer *renderer):
    renderer(renderer)
{
}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
}

void VulkanFrameBuffer::setAttachments(const vec<sptr<Texture2d>> &attachments)
{
    SL_PANIC_BLOCK(validateNewAttachments(attachments));

    if (attachments.empty())
    {
        // Revert to empty state
        if (frameBuffer)
            frameBuffer.cleanRef();
        renderPass = VulkanRenderPass();
        depthStencil = VulkanImage();
        return;
    }

    vec<VkImageView> attachmentViews;
    VulkanRenderPassConfig config{};
    for (const auto tex: attachments)
    {
        const auto vulkanTexture = std::static_pointer_cast<VulkanTexture2d>(tex);
        config.withColorAttachment(vulkanTexture->getImage().getFormat(), VK_IMAGE_LAYOUT_GENERAL, true, {0, 0, 0, 1}); // TODO Proper layout
        attachmentViews.push_back(vulkanTexture->getImage().getView());
    }

    const auto size = attachments[0]->getDimensions();
    
    depthStencil = VulkanImage(
        renderer,
        size.x, size.y,
        1, 1,
        renderer->getDepthFormat(),
        0,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);

    config.withDepthAttachment(renderer->getDepthFormat(), true, {1, 0});
    attachmentViews.push_back(depthStencil.getView());
    
    renderPass = VulkanRenderPass(renderer->getDevice(), config);
    frameBuffer = vk::createFrameBuffer(renderer->getDevice(), attachmentViews, renderPass, size.x, size.y);
}

#endif
