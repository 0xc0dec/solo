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
            SL_PANIC("Unsupported image format");
            return VK_FORMAT_UNDEFINED;
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

static auto createSampler(VkDevice device, VkPhysicalDeviceFeatures physicalFeatures, VkPhysicalDeviceProperties physicalProps,
    u32 mipLevels) -> VulkanResource<VkSampler>
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.maxAnisotropy = 1.0f;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = mipLevels;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    if (physicalFeatures.samplerAnisotropy)
    {
        samplerInfo.maxAnisotropy = physicalProps.limits.maxSamplerAnisotropy;
        samplerInfo.anisotropyEnable = VK_TRUE;
    }

    VulkanResource<VkSampler> sampler{device, vkDestroySampler};
    SL_VK_CHECK_RESULT(vkCreateSampler(device, &samplerInfo, nullptr, sampler.cleanRef()));

    return sampler;
}

static void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
    VkImageSubresourceRange subresourceRange, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask)
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
auto VulkanImage::create2d(VulkanRenderer *renderer, Texture2dData *data) -> VulkanImage
{
    const auto mipLevels = 1; // TODO proper support
    const auto layers = 1;
    const auto width = data->getWidth();
    const auto height = data->getHeight();
    const auto format = toVulkanFormat(data->getFormat());

    auto image = VulkanImage(
        renderer, width, height, mipLevels, 1,
        format,
        0,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_IMAGE_ASPECT_COLOR_BIT);
    
    u32 offset = 0;
    vec<VkBufferImageCopy> copyRegions;
    for (u32 layer = 0; layer < 1; layer++) // Layers == 1 because 2d
    {
        for (u32 level = 0; level < mipLevels; level++)
        {
            VkBufferImageCopy bufferCopyRegion{};
            bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.mipLevel = level;
            bufferCopyRegion.imageSubresource.baseArrayLayer = layer;
            bufferCopyRegion.imageSubresource.layerCount = 1;
            bufferCopyRegion.imageExtent.width = data->getWidth(); // TODO use per-level dimensions once TextureData supports mip levels
            bufferCopyRegion.imageExtent.height = data->getHeight(); // TODO use per-level dimensions once TextureData supports mip levels
            bufferCopyRegion.imageExtent.depth = 1;
            bufferCopyRegion.bufferOffset = offset;

            copyRegions.push_back(bufferCopyRegion);

            offset += data->getSize(); // TODO use per-level size once TextureData supports mip levels
        }
    }

    VkImageSubresourceRange subresourceRange{};
	subresourceRange.aspectMask = image.aspectMask;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = mipLevels;
	subresourceRange.layerCount = layers;

    auto cmdBuf = vk::createCommandBuffer(renderer->getDevice(), renderer->getCommandPool());
    vk::beginCommandBuffer(cmdBuf, true);

    const auto size = data->getSize();
    if (size)
    {
        auto srcBuf = VulkanBuffer::createStaging(renderer, data->getSize(), data->getData());
        setImageLayout(
            cmdBuf,
            image.image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            subresourceRange,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);

        vkCmdCopyBufferToImage(
            cmdBuf,
            srcBuf,
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
    else
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
    layers(layers),
    width(width),
    height(height),
    aspectMask(aspectMask)
{
    const auto device = renderer->getDevice();
    auto image = createImage(device, format, width, height, mipLevels, layers, createFlags, usageFlags);
    auto memory = allocateImageMemory(device, renderer->getPhysicalMemoryFeatures(), image);
    auto sampler = createSampler(device, renderer->getPhysicalFeatures(), renderer->getPhysicalProperties(), mipLevels);
    auto view = vk::createImageView(device, format, viewType, mipLevels, layers, image, aspectMask);
    this->image = std::move(image);
    this->memory = std::move(memory);
    this->sampler = std::move(sampler);
    this->view = std::move(view);
}

#endif