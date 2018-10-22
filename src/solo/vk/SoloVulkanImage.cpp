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
        case TextureFormat::R8: return VK_FORMAT_R8_UNORM;
        case TextureFormat::RGB8: // TODO real 24-bit? They crash my driver...
        case TextureFormat::RGBA8: return VK_FORMAT_R8G8B8A8_UNORM;
        case TextureFormat::RGBA16F: return VK_FORMAT_R16G16B16A16_SFLOAT;
        case TextureFormat::Depth24: return VK_FORMAT_D32_SFLOAT; // TODO real 24-bit depth?
        default:
            break;
    }

    SL_DEBUG_PANIC(true, "Unsupported texture format");
    return VK_FORMAT_UNDEFINED;
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

// TODO Refactor, reduce copy-paste
auto VulkanImage::createEmpty2D(VulkanRenderer *renderer, u32 width, u32 height, TextureFormat format) -> VulkanImage
{
    const auto isDepth = format == TextureFormat::Depth24;
    const auto targetLayout = isDepth ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    const auto colorOrDepthUsage = isDepth ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    const auto usage = VK_IMAGE_USAGE_SAMPLED_BIT | colorOrDepthUsage;
    const auto aspect = isDepth ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    const auto format_ = toVulkanFormat(format);

    // TODO Better check. Checking for color attachment and sampled bits seems not right or too general
    SL_DEBUG_PANIC(
        !vk::isFormatSupported(
            renderer->getPhysicalDevice(),
            format_,
            VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | (isDepth ? VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)
        ),
        "Image format/features not supported"
    );

    auto image = VulkanImage(
        renderer,
        width, height,
        1, 1,
        format_,
        targetLayout,
        0,
        usage,
        VK_IMAGE_VIEW_TYPE_2D,
        aspect);

    const auto initCmdBuf = vk::createCommandBuffer(renderer->device(), renderer->commandPool(), true);

    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = image.aspectMask;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    vk::setImageLayout(
        initCmdBuf,
        image.image,
        VK_IMAGE_LAYOUT_UNDEFINED,
        targetLayout,
        subresourceRange,
        VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, // TODO veeery unsure about this, but validator doesn't complain
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

    vk::flushCommandBuffer(initCmdBuf, renderer->queue());

    return image;
}

// TODO Refactor, reduce copy-paste
auto VulkanImage::create2D(VulkanRenderer *renderer, Texture2DData *data, bool generateMipmaps) -> VulkanImage
{
    const auto width = static_cast<u32>(data->getDimensions().x());
    const auto height = static_cast<u32>(data->getDimensions().y());
    const auto format = toVulkanFormat(data->getTextureFormat());
    const auto targetLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    auto usage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
        VK_IMAGE_USAGE_SAMPLED_BIT |
        VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    SL_DEBUG_PANIC(
        !vk::isFormatSupported(renderer->getPhysicalDevice(), format,
            VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT |
            VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT |
            VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR),
            "Image format/features not supported"
        );

    u32 mipLevels = 1;
    if (generateMipmaps)
    {
        SL_DEBUG_PANIC(
            !vk::isFormatSupported(renderer->getPhysicalDevice(), format, VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT),
            "Image format/features not supported"
        );
        mipLevels = static_cast<u32>(std::floorf(std::log2f((std::fmax)(static_cast<float>(width), static_cast<float>(height))))) + 1;
        usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }

    auto image = VulkanImage(
        renderer,
        width, height,
        mipLevels, 1,
        format,
        targetLayout,
        0,
        usage,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_IMAGE_ASPECT_COLOR_BIT);

    const auto initCmdBuf = vk::createCommandBuffer(renderer->device(), renderer->commandPool(), true);

    VkBufferImageCopy bufferCopyRegion{};
    bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    bufferCopyRegion.imageSubresource.mipLevel = 0;
    bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
    bufferCopyRegion.imageSubresource.layerCount = 1;
    bufferCopyRegion.imageExtent.width = width;
    bufferCopyRegion.imageExtent.height = height;
    bufferCopyRegion.imageExtent.depth = 1;
    bufferCopyRegion.bufferOffset = 0;

    auto srcBuf = VulkanBuffer::createStaging(renderer, data->getSize(), data->getData());

    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = image.aspectMask;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    vk::setImageLayout(
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

    if (generateMipmaps)
    {
        vk::setImageLayout(
            initCmdBuf,
            image.image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            subresourceRange,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT);

        vk::flushCommandBuffer(initCmdBuf, renderer->queue());

        const auto blitCmdBuf = vk::createCommandBuffer(renderer->device(), renderer->commandPool(), true);

        for (u32 i = 1; i < mipLevels; i++)
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

            vk::setImageLayout(
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

            vk::setImageLayout(
                blitCmdBuf,
                image.image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                mipSubRange,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT);
        }

        subresourceRange.levelCount = static_cast<u32>(mipLevels);
        vk::setImageLayout(
            blitCmdBuf,
            image.image,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            targetLayout,
            subresourceRange,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

        vk::flushCommandBuffer(blitCmdBuf, renderer->queue());
    }
    else
    {
        vk::setImageLayout(
            initCmdBuf,
            image.image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            targetLayout,
            subresourceRange,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

        vk::flushCommandBuffer(initCmdBuf, renderer->queue());
    }

    return image;
}

auto VulkanImage::createCube(VulkanRenderer *renderer, CubeTextureData *data) -> VulkanImage
{
    const u32 mipLevels = 1; // TODO proper support
    const auto layers = 6;
    const auto width = data->getDimension();
    const auto height = width;
    const auto format = toVulkanFormat(data->getTextureFormat());
    const auto targetLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    const auto usage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
        VK_IMAGE_USAGE_SAMPLED_BIT |
        VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    SL_DEBUG_PANIC(
        !vk::isFormatSupported(renderer->getPhysicalDevice(), format,
            VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT |
            VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT |
            VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR),
            "Image format/features not supported"
        );

    auto image = VulkanImage(
        renderer,
        width, height, mipLevels, layers,
        format,
        targetLayout,
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        usage,
        VK_IMAGE_VIEW_TYPE_CUBE,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = image.aspectMask;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = mipLevels;
    subresourceRange.layerCount = layers;

    const auto cmdBuf = vk::createCommandBuffer(renderer->device(), renderer->commandPool());
    vk::beginCommandBuffer(cmdBuf, true);

    vk::setImageLayout(
        cmdBuf,
        image.image,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        subresourceRange,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT);

    auto srcBuffer = VulkanBuffer::createStaging(renderer, data->getSize());

    // Engine provides faces in order +X, -X, +Y, -Y, +Z, -Z
    // Vulkan's Y axis is inverted, so we invert
    static vec<u32> layerFaceMapping = {0, 1, 3, 2, 4, 5};
        
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
        static_cast<u32>(copyRegions.size()),
        copyRegions.data());

    vk::setImageLayout(
        cmdBuf,
        image.image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        targetLayout,
        subresourceRange,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

    vk::flushCommandBuffer(cmdBuf, renderer->queue());

    return image;
}

VulkanImage::VulkanImage(VulkanRenderer *renderer, u32 width, u32 height, u32 mipLevels, u32 layers, VkFormat format, VkImageLayout layout,
    VkImageCreateFlags createFlags, VkImageUsageFlags usageFlags, VkImageViewType viewType, VkImageAspectFlags aspectMask):
    layout(layout),
    format(format),
    mipLevels(mipLevels),
    width(width),
    height(height),
    aspectMask(aspectMask)
{
    const auto device = renderer->device();
    auto image = createImage(device, format, width, height, mipLevels, layers, createFlags, usageFlags);
    auto memory = allocateImageMemory(device, renderer->physicalMemoryFeatures(), image);
    auto view = vk::createImageView(device, format, viewType, mipLevels, layers, image, aspectMask);
    this->image = std::move(image);
    this->memory = std::move(memory);
    this->view = std::move(view);
}

#endif