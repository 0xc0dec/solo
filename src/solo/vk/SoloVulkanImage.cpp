/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanImage.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanRenderer.h"
#include "SoloTexture.h"
#include "SoloVulkanBuffer.h"
#include "SoloTextureData.h"

using namespace solo;

static auto toVulkanFormat(TextureFormat format) -> VkFormat
{
    switch (format)
    {
        case TextureFormat::RGB:
        case TextureFormat::RGBA: return VK_FORMAT_R8G8B8A8_UNORM; // since my driver seems not liking 24-bit
        case TextureFormat::Red: return VK_FORMAT_R8_UNORM;
        default:
            return panic<VkFormat>("Unsupported image format");
    }
}

static auto createImage(VkDevice device, VkFormat format, u32 width, u32 height, u32 mipLevels,
    u32 arrayLayers, VkImageCreateFlags createFlags, VkImageUsageFlags usageFlags) -> VulkanResource<VkImage>
{
    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = format;
    imageCreateInfo.mipLevels = mipLevels;
    imageCreateInfo.arrayLayers = arrayLayers;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.extent = {width, height, 1};
    imageCreateInfo.usage = usageFlags;
    imageCreateInfo.flags = createFlags;

    VulkanResource<VkImage> image{device, vkDestroyImage};
    SL_VK_CHECK_RESULT(vkCreateImage(device, &imageCreateInfo, nullptr, image.cleanRef()));

    return image;
}

static void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
    VkImageSubresourceRange subresourceRange,
    VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT)
{
    VkImageMemoryBarrier imageMemoryBarrier{};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.oldLayout = oldImageLayout;
    imageMemoryBarrier.newLayout = newImageLayout;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = subresourceRange;

    switch (oldImageLayout)
    {
        case VK_IMAGE_LAYOUT_UNDEFINED:
            imageMemoryBarrier.srcAccessMask = 0;
            break;

        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;
        default:
            break;
    }

    switch (newImageLayout)
    {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            if (imageMemoryBarrier.srcAccessMask == 0)
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;

        default:
            break;
    }

    vkCmdPipelineBarrier(cmdbuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
}

static auto allocateImageMemory(VkDevice device, VkPhysicalDeviceMemoryProperties memProps, VkImage image) -> VulkanResource<VkDeviceMemory>
{
    VkMemoryRequirements memReqs{};
    vkGetImageMemoryRequirements(device, image, &memReqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = vk::findMemoryType(memProps, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VulkanResource<VkDeviceMemory> memory{device, vkFreeMemory};
    SL_VK_CHECK_RESULT(vkAllocateMemory(device, &allocInfo, nullptr, memory.cleanRef()));
    SL_VK_CHECK_RESULT(vkBindImageMemory(device, image, memory, 0));

    return memory;
}

// TODO Refactor, avoid copy-paste
auto VulkanImage::create2d(VulkanRenderer *renderer, Texture2dData *data, bool generateMipmaps) -> VulkanImage
{
    const auto width = data->getWidth();
    const auto height = data->getHeight();
    const auto size = data->getSize();
    const auto format = toVulkanFormat(data->getFormat());
    const auto withMipmaps = generateMipmaps && size; // generating mips for non-empty textures

    auto mipLevels = 1;
    if (withMipmaps)
    {
        SL_DEBUG_BLOCK({
            VkFormatProperties formatProperties;
            vkGetPhysicalDeviceFormatProperties(renderer->getPhysicalDevice(), format, &formatProperties);
            panicIf(!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT));
            panicIf(!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT));
        });

        mipLevels = floor(log2((std::max)(width, height))) + 1;
    }

    auto image = VulkanImage(
        renderer,
        width, height,
        mipLevels, 1,
        format,
        0,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_IMAGE_ASPECT_COLOR_BIT);
    
    auto initCmdBuf = vk::createCommandBuffer(renderer->getDevice(), renderer->getCommandPool());
    vk::beginCommandBuffer(initCmdBuf, true);

    if (size)
    {
        VkBufferImageCopy bufferCopyRegion{};
        bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        bufferCopyRegion.imageSubresource.mipLevel = 0;
        bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
        bufferCopyRegion.imageSubresource.layerCount = 1;
        bufferCopyRegion.imageExtent.width = data->getWidth();
        bufferCopyRegion.imageExtent.height = data->getHeight();
        bufferCopyRegion.imageExtent.depth = 1;
        bufferCopyRegion.bufferOffset = 0;

        auto srcBuf = VulkanBuffer::createStaging(renderer, data->getSize(), data->getData());

        VkImageSubresourceRange subresourceRange{};
	    subresourceRange.aspectMask = image.aspectMask;
	    subresourceRange.baseArrayLayer = 0;
	    subresourceRange.baseMipLevel = 0;
	    subresourceRange.levelCount = 1;
	    subresourceRange.layerCount = 1;

        setImageLayout(
            initCmdBuf,
            image.image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            subresourceRange,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT);

        vkCmdCopyBufferToImage(
            initCmdBuf,
            srcBuf,
            image.image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &bufferCopyRegion);

        if (withMipmaps)
        {
            setImageLayout(
                initCmdBuf,
                image.image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                subresourceRange,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT);

            // TODO Refactor, avoid copy-paste in other similar places
            vkEndCommandBuffer(initCmdBuf);
            vk::queueSubmit(renderer->getQueue(), 0, nullptr, 0, nullptr, 1, &initCmdBuf);
            SL_VK_CHECK_RESULT(vkQueueWaitIdle(renderer->getQueue()));

            const auto blitCmdBuf = vk::createCommandBuffer(renderer->getDevice(), renderer->getCommandPool());
            vk::beginCommandBuffer(blitCmdBuf, true);

            for (s32 i = 1; i < mipLevels; i++)
            {
                VkImageBlit imageBlit{};				

			    // Source
			    imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			    imageBlit.srcSubresource.layerCount = 1;
			    imageBlit.srcSubresource.mipLevel = i - 1;
			    imageBlit.srcOffsets[1].x = s32(width >> (i - 1));
			    imageBlit.srcOffsets[1].y = s32(height >> (i - 1));
			    imageBlit.srcOffsets[1].z = 1;

			    // Destination
			    imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			    imageBlit.dstSubresource.layerCount = 1;
			    imageBlit.dstSubresource.mipLevel = i;
			    imageBlit.dstOffsets[1].x = s32(width >> i);
			    imageBlit.dstOffsets[1].y = s32(height >> i);
			    imageBlit.dstOffsets[1].z = 1;

			    VkImageSubresourceRange mipSubRange{};
			    mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			    mipSubRange.baseMipLevel = i;
			    mipSubRange.levelCount = 1;
			    mipSubRange.layerCount = 1;

                setImageLayout(
                    blitCmdBuf,
                    image.image,
                    VK_IMAGE_LAYOUT_UNDEFINED,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    mipSubRange,
                    VK_PIPELINE_STAGE_TRANSFER_BIT,
                    VK_PIPELINE_STAGE_TRANSFER_BIT);

                vkCmdBlitImage(
				    blitCmdBuf,
				    image.image,
				    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				    image.image,
				    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				    1,
				    &imageBlit,
				    VK_FILTER_LINEAR);

                setImageLayout(
                    blitCmdBuf,
                    image.image,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    mipSubRange,
                    VK_PIPELINE_STAGE_TRANSFER_BIT,
                    VK_PIPELINE_STAGE_TRANSFER_BIT);
            }

            subresourceRange.levelCount = mipLevels;
            setImageLayout(
                blitCmdBuf,
                image.image,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                subresourceRange,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

            vkEndCommandBuffer(blitCmdBuf);
            vk::queueSubmit(renderer->getQueue(), 0, nullptr, 0, nullptr, 1, &blitCmdBuf);
            SL_VK_CHECK_RESULT(vkQueueWaitIdle(renderer->getQueue()));
        }
        else
        {
            setImageLayout(
                initCmdBuf,
                image.image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                subresourceRange,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

            vkEndCommandBuffer(initCmdBuf);
            vk::queueSubmit(renderer->getQueue(), 0, nullptr, 0, nullptr, 1, &initCmdBuf);
            SL_VK_CHECK_RESULT(vkQueueWaitIdle(renderer->getQueue()));
        }
    }
    else // blank texture
    {
        VkImageSubresourceRange subresourceRange{};
	    subresourceRange.aspectMask = image.aspectMask;
	    subresourceRange.baseMipLevel = 0;
	    subresourceRange.levelCount = 1;
	    subresourceRange.layerCount = 1;

        setImageLayout(
            initCmdBuf,
            image.image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            subresourceRange,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, // TODO veeery unsure about this, but validator doesn't complain
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

        vkEndCommandBuffer(initCmdBuf);
        vk::queueSubmit(renderer->getQueue(), 0, nullptr, 0, nullptr, 1, &initCmdBuf);
        SL_VK_CHECK_RESULT(vkQueueWaitIdle(renderer->getQueue()));
    }

    return image;
}

auto VulkanImage::createCube(VulkanRenderer *renderer, CubeTextureData *data) -> VulkanImage
{
    const auto mipLevels = 1; // TODO proper support
    const auto layers = 6;
    const auto width = data->getDimension();
    const auto height = width;
    const auto format = toVulkanFormat(data->getFormat());

    auto image = VulkanImage(
        renderer,
        width, height, mipLevels, layers,
        format,
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_VIEW_TYPE_CUBE,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    VkImageSubresourceRange subresourceRange{};
	subresourceRange.aspectMask = image.aspectMask;
	subresourceRange.baseArrayLayer = 0;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = mipLevels;
	subresourceRange.layerCount = layers;

    auto cmdBuf = vk::createCommandBuffer(renderer->getDevice(), renderer->getCommandPool());
    vk::beginCommandBuffer(cmdBuf, true);

    const auto size = data->getSize();
    if (size)
    {
        setImageLayout(
            cmdBuf,
            image.image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            subresourceRange,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);

        auto srcBuffer = VulkanBuffer::createStaging(renderer, data->getSize());

        // We load front, back, etc., this expects +x, -x, +y, -y, +z, -z,
        // also in Vulkan Y axis is inversed, so we end up with
        // left, right, down, up, front, back
        static vec<u32> layerFaceMapping = {2, 3, 5, 4, 0, 1};
        
        u32 offset = 0;
        vec<VkBufferImageCopy> copyRegions;
        for (u32 layer = 0; layer < layers; layer++)
        {
            srcBuffer.updatePart(data->getData(layerFaceMapping[layer]), offset, data->getSize(layerFaceMapping[layer]));

            for (u32 level = 0; level < mipLevels; level++)
            {
                VkBufferImageCopy bufferCopyRegion{};
                bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                bufferCopyRegion.imageSubresource.mipLevel = level;
                bufferCopyRegion.imageSubresource.baseArrayLayer = layer;
                bufferCopyRegion.imageSubresource.layerCount = 1;
                bufferCopyRegion.imageExtent.width = data->getDimension();
                bufferCopyRegion.imageExtent.height = data->getDimension();
                bufferCopyRegion.imageExtent.depth = 1;
                bufferCopyRegion.bufferOffset = offset;

                copyRegions.push_back(bufferCopyRegion);

                offset += data->getSize(layer); // TODO use per-level size once TextureData supports mip levels
            }
        }

        vkCmdCopyBufferToImage(
            cmdBuf,
            srcBuffer,
            image.image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            copyRegions.size(),
            copyRegions.data());

        setImageLayout(
            cmdBuf,
            image.image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            subresourceRange,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    }
    else // empty
    {
        setImageLayout(
            cmdBuf,
            image.image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            subresourceRange,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    }

    vkEndCommandBuffer(cmdBuf);

    vk::queueSubmit(renderer->getQueue(), 0, nullptr, 0, nullptr, 1, &cmdBuf);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(renderer->getQueue()));

    return image;
}

VulkanImage::VulkanImage(VulkanRenderer *renderer, u32 width, u32 height, u32 mipLevels, u32 layers, VkFormat format,
    VkImageCreateFlags createFlags, VkImageUsageFlags usageFlags, VkImageViewType viewType, VkImageAspectFlags aspectMask):
    format(format),
    mipLevels(mipLevels),
    width(width),
    height(height),
    aspectMask(aspectMask)
{
    const auto device = renderer->getDevice();
    auto image = createImage(device, format, width, height, mipLevels, layers, createFlags, usageFlags);
    auto memory = allocateImageMemory(device, renderer->getPhysicalMemoryFeatures(), image);
    auto view = vk::createImageView(device, format, viewType, mipLevels, layers, image, aspectMask);
    this->image = std::move(image);
    this->memory = std::move(memory);
    this->view = std::move(view);
}

#endif