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
	panicIf(attachments.empty(), "Frame buffer must have at least one attachment"); // TODO is it a temp check?

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

auto VulkanFrameBuffer::create(Device *device, const vec<sptr<Texture2D>> &attachments) -> sptr<VulkanFrameBuffer>
{
	SL_DEBUG_BLOCK(validateNewAttachments(attachments));

    vec<VkImageView> views;
    VulkanRenderPassConfig config;
    for (const auto &tex: attachments)
    {
        const auto vulkanTexture = std::static_pointer_cast<VulkanTexture2D>(tex);
        config.withColorAttachment(vulkanTexture->getImage().getFormat(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL); // TODO Proper layout
        views.push_back(vulkanTexture->getImage().getView());
    }

	auto result = sptr<VulkanFrameBuffer>(new VulkanFrameBuffer());

	const auto renderer = static_cast<VulkanRenderer*>(device->getRenderer());

    result->dimensions = attachments[0]->getDimensions();
    result->depthStencil = VulkanImage(
        renderer,
        result->dimensions.x(), result->dimensions.y(),
        1, 1,
        renderer->getDepthFormat(),
        0,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);

    config.withDepthAttachment(renderer->getDepthFormat());
    views.push_back(result->depthStencil.getView());

    result->renderPass = VulkanRenderPass(renderer->getDevice(), config);
    result->frameBuffer = vk::createFrameBuffer(renderer->getDevice(), views, result->renderPass, result->dimensions.x(), result->dimensions.y());
    result->colorAttachmentCount = attachments.size();

	return result;
}

#endif
