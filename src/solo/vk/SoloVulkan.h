/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanResource.h"

#ifdef SL_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan.h>

#ifdef SL_DEBUG
#   define SL_VK_CHECK_RESULT(vkCall) SL_DEBUG_PANIC(vkCall != VK_SUCCESS, "Vulkan API call failed")
#else
#   define SL_VK_CHECK_RESULT(vkCall) vkCall
#endif

namespace solo
{
    enum class TextureFormat;

    namespace vk
    {
        auto createSemaphore(VkDevice device) -> VulkanResource<VkSemaphore>;
        void queueSubmit(VkQueue queue, u32 waitSemaphoreCount, const VkSemaphore *waitSemaphores,
            u32 signalSemaphoreCount, const VkSemaphore *signalSemaphores,
            u32 commandBufferCount, const VkCommandBuffer *commandBuffers);
        auto findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, u32 typeBits,
            VkMemoryPropertyFlags properties) -> s32;
        auto createFrameBuffer(VkDevice device, const vec<VkImageView> &attachments,
            VkRenderPass renderPass, u32 width, u32 height) -> VulkanResource<VkFramebuffer>;
        auto createImageView(VkDevice device, VkFormat format, VkImageViewType type, u32 mipLevels, u32 layers,
            VkImage image, VkImageAspectFlags aspectMask) -> VulkanResource<VkImageView>;
        auto makeImagePipelineBarrier(VkImage image, VkImageLayout oldImageLayout,
            VkImageLayout newImageLayout, VkImageSubresourceRange subresourceRange) -> VkImageMemoryBarrier;
        bool isFormatSupported(VkPhysicalDevice device, VkFormat format, VkFormatFeatureFlags features);
    }
}

#endif
