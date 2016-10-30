#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#ifdef SL_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR 
#endif
#include <vulkan.h>

#ifdef SL_ERR_CHECK
#   define SL_CHECK_VK_RESULT(vkCall, ...) SL_BLOCK(if (vkCall != VK_SUCCESS) SL_ERR(__VA_ARGS__))
#else
#   define SL_CHECK_VK_RESULT(vkCall) vkCall
#endif

namespace solo
{
    namespace vk
    {
        auto createDevice(VkPhysicalDevice physicalDevice, uint32_t queueIndex) -> VkDevice;
        auto getPhysicalDevice(VkInstance instance) -> VkPhysicalDevice;
        auto getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface) -> uint32_t;
        auto getDepthFormat(VkPhysicalDevice device) -> VkFormat;
        auto createCommandPool(VkDevice logicalDevice, uint32_t queueIndex) -> VkCommandPool;
        void submitCommandBuffer(VkQueue queue, VkCommandBuffer buffer);
        auto createCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool) -> VkCommandBuffer;
        auto findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, uint32_t typeBits,
            VkMemoryPropertyFlags properties) -> int32_t;
        void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldLayout,
            VkImageLayout newLayout, VkImageSubresourceRange subresourceRange);
        void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask,
            VkImageLayout oldLayout, VkImageLayout newLayout);
    }
}

#else
#   error Vulkan renderer is not supported on this platform
#endif
