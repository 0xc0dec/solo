/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanFrameBuffer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloTextureData.h"
#include "SoloVulkanRenderer.h"
#include "SoloVulkanTexture.h"

using namespace solo;

// TODO Refactor, this logic repeats that from OpenGL
static void validateNewAttachments(const vec<sptr<Texture2D>> &attachments)
{
	panicIf(attachments.empty(), "Frame buffer must have at least one attachment"); // TODO is it a temp check?

    auto width = -1, height = -1;
    for (const auto &attachment : attachments)
    {
	    const auto size = attachment->getDimensions();
        if (width < 0)
        {
            width = size.x();
            height = size.y();
        }
        else
            panicIf(size.x() != width || size.y() != height, "Attachment sizes do not match");
    }
}

auto VulkanFrameBuffer::create(Device *device, const vec<sptr<Texture2D>> &attachments) -> sptr<VulkanFrameBuffer>
{
	SL_DEBUG_BLOCK(validateNewAttachments(attachments));
	// TODO validate that not more that 1 depth attachment

	const auto renderer = static_cast<VulkanRenderer*>(device->getRenderer());
	auto result = sptr<VulkanFrameBuffer>(new VulkanFrameBuffer());

    vec<VkImageView> views;
    VulkanRenderPassConfig config;

    for (const auto &tex: attachments)
    {
        const auto vkTexture = std::static_pointer_cast<VulkanTexture2D>(tex);
		if (tex->getFormat() == TextureFormat::Depth)
			result->depthAttachment = vkTexture;
		else
		{
			result->colorAttachments.push_back(vkTexture);
			config.withColorAttachment(vkTexture->getImage().getFormat(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL); // TODO Proper layout
		}
        views.push_back(vkTexture->getImage().getView());
    }

    result->dimensions = attachments[0]->getDimensions();

	if (!result->depthAttachment)
	{
		auto data = Texture2DData::createFromMemory(result->dimensions.x(), result->dimensions.y(), TextureFormat::Depth, vec<u8>{});
		result->depthAttachment = VulkanTexture2D::createFromData(device, data.get(), false);
		config.withDepthAttachment(renderer->getDepthFormat());
		views.push_back(result->depthAttachment->getImage().getView());
	}

	config.withDepthAttachment(result->depthAttachment->getImage().getFormat());

    result->renderPass = VulkanRenderPass(renderer->getDevice(), config);
    result->frameBuffer = vk::createFrameBuffer(renderer->getDevice(), views, result->renderPass, result->dimensions.x(), result->dimensions.y());

	return result;
}

#endif
