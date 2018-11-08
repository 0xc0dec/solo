/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkan.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;

auto vk::createSemaphore(VkDevice device) -> VulkanResource<VkSemaphore>
{
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = nullptr;
    semaphoreCreateInfo.flags = 0;

    VulkanResource<VkSemaphore> semaphore{device, vkDestroySemaphore};
    SL_VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, semaphore.cleanRef()));

    return semaphore;
}

auto vk::createCommandBuffer(VkDevice device, VkCommandPool commandPool, bool begin) -> VulkanResource<VkCommandBuffer>
{
    VkCommandBufferAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.pNext = nullptr;
    allocateInfo.commandPool = commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    VulkanResource<VkCommandBuffer> buffer{device, commandPool, vkFreeCommandBuffers};
    SL_VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &allocateInfo, &buffer));

    if (begin)
        vk::beginCommandBuffer(buffer, true);

    return buffer;
}

void vk::beginCommandBuffer(VkCommandBuffer buffer, bool oneTime)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = oneTime ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : 0;
    SL_VK_CHECK_RESULT(vkBeginCommandBuffer(buffer, &beginInfo));
}

void vk::flushCommandBuffer(VkCommandBuffer buffer, VkQueue queue)
{
    SL_VK_CHECK_RESULT(vkEndCommandBuffer(buffer));
    queueSubmit(queue, 0, nullptr, 0, nullptr, 1, &buffer);
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(queue));
}

void vk::queueSubmit(VkQueue queue, u32 waitSemaphoreCount, const VkSemaphore *waitSemaphores,
    u32 signalSemaphoreCount, const VkSemaphore *signalSemaphores,
    u32 commandBufferCount, const VkCommandBuffer *commandBuffers)
{
    VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
    submitInfo.waitSemaphoreCount = waitSemaphoreCount;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.signalSemaphoreCount = signalSemaphoreCount;
    submitInfo.pSignalSemaphores = signalSemaphores;
    submitInfo.commandBufferCount = commandBufferCount;
    submitInfo.pCommandBuffers = commandBuffers;
    SL_VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
}

auto vk::findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, u32 typeBits,
    VkMemoryPropertyFlags properties) -> s32
{
    for (u32 i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((typeBits & 1) == 1)
        {
            if ((physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }
        typeBits >>= 1;
    }
    return -1;
}

auto vk::createFrameBuffer(VkDevice device, const vec<VkImageView> &attachments,
    VkRenderPass renderPass, u32 width, u32 height) -> VulkanResource<VkFramebuffer>
{
    VkFramebufferCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.renderPass = renderPass;
    createInfo.attachmentCount = attachments.size();
    createInfo.pAttachments = attachments.data();
    createInfo.width = width;
    createInfo.height = height;
    createInfo.layers = 1;

    VulkanResource<VkFramebuffer> frameBuffer{device, vkDestroyFramebuffer};
    SL_VK_CHECK_RESULT(vkCreateFramebuffer(device, &createInfo, nullptr, frameBuffer.cleanRef()));

    return frameBuffer;
}

auto vk::createImageView(VkDevice device, VkFormat format, VkImageViewType type, u32 mipLevels, u32 layers,
    VkImage image, VkImageAspectFlags aspectMask) -> VulkanResource<VkImageView>
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.viewType = type;
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.format = format;
    viewInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
    viewInfo.subresourceRange = {};
    viewInfo.subresourceRange.aspectMask = aspectMask;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = layers;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.image = image;

    VulkanResource<VkImageView> view{device, vkDestroyImageView};
    SL_VK_CHECK_RESULT(vkCreateImageView(device, &viewInfo, nullptr, view.cleanRef()));

    return view;
}

void vk::setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldImageLayout,
    VkImageLayout newImageLayout, VkImageSubresourceRange subresourceRange, VkPipelineStageFlags srcStageMask,
    VkPipelineStageFlags dstStageMask)
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

static void detectFormatSupport(VkPhysicalDevice device, VkFormat format, umap<VkFormat, VkFormatFeatureFlags> &supportedFormats)
{
    // TODO Check for linear tiling as well
    VkFormatProperties formatProps;
    vkGetPhysicalDeviceFormatProperties(device, format, &formatProps);
    if (formatProps.optimalTilingFeatures)
        supportedFormats[format] = formatProps.optimalTilingFeatures;
}

bool vk::isFormatSupported(VkPhysicalDevice device, VkFormat format, VkFormatFeatureFlags features)
{
    static umap<VkFormat, VkFormatFeatureFlags> supportedFormats;
    static auto initialized = false;
    
    if (!initialized)
    {
        detectFormatSupport(device, VK_FORMAT_R8_UNORM, supportedFormats);
        detectFormatSupport(device, VK_FORMAT_R8G8B8A8_UNORM, supportedFormats);
        detectFormatSupport(device, VK_FORMAT_R16G16B16A16_SFLOAT, supportedFormats);
        detectFormatSupport(device, VK_FORMAT_D32_SFLOAT, supportedFormats);
        detectFormatSupport(device, VK_FORMAT_D32_SFLOAT_S8_UINT, supportedFormats);
        detectFormatSupport(device, VK_FORMAT_D24_UNORM_S8_UINT, supportedFormats);
        detectFormatSupport(device, VK_FORMAT_D16_UNORM_S8_UINT, supportedFormats);
        detectFormatSupport(device, VK_FORMAT_D16_UNORM, supportedFormats);
        initialized = true;
    }

    return supportedFormats.count(format) && (supportedFormats.at(format) & features) == features;
}

#endif
