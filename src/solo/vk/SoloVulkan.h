/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
#   define SL_VK_CHECK_RESULT(vkCall) SL_DEBUG_PANIC(vkCall != VK_SUCCESS, "Unable to complete VK call")
#else
#   define SL_VK_CHECK_RESULT(vkCall) vkCall
#endif

namespace solo
{
    enum class TextureFormat;

    namespace vk
    {
        auto createSemaphore(VkDevice device) -> VulkanResource<VkSemaphore>;
        auto createCommandBuffer(VkDevice device, VkCommandPool commandPool, bool begin = false) -> VulkanResource<VkCommandBuffer>;
        void beginCommandBuffer(VkCommandBuffer buffer, bool oneTime);
        void flushCommandBuffer(VkCommandBuffer buffer, VkQueue queue);
        void queueSubmit(VkQueue queue, u32 waitSemaphoreCount, const VkSemaphore *waitSemaphores,
            u32 signalSemaphoreCount, const VkSemaphore *signalSemaphores,
            u32 commandBufferCount, const VkCommandBuffer *commandBuffers);
        auto findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, u32 typeBits,
            VkMemoryPropertyFlags properties) -> s32;
        auto createFrameBuffer(VkDevice device, const vec<VkImageView> &attachments,
            VkRenderPass renderPass, u32 width, u32 height) -> VulkanResource<VkFramebuffer>;
        auto createImageView(VkDevice device, VkFormat format, VkImageViewType type, u32 mipLevels, u32 layers,
            VkImage image, VkImageAspectFlags aspectMask) -> VulkanResource<VkImageView>;
        void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
            VkImageSubresourceRange subresourceRange,
            VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
        bool isFormatSupported(VkPhysicalDevice device, VkFormat format, VkFormatFeatureFlags features);
    }
}

#endif
