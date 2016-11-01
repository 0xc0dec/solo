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
        void destroyCommandBuffers(VkDevice device, VkCommandPool commandPool, VkCommandBuffer* buffers, uint32_t count);
        auto findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, uint32_t typeBits,
            VkMemoryPropertyFlags properties) -> int32_t;
        void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldLayout,
            VkImageLayout newLayout, VkImageSubresourceRange subresourceRange);
        void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask,
            VkImageLayout oldLayout, VkImageLayout newLayout);
        auto createRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat) -> VkRenderPass;
        auto createFrameBuffer(VkDevice device, VkImageView colorAttachment, VkImageView depthAttachment,
            VkRenderPass renderPass, uint32_t width, uint32_t height) -> VkFramebuffer;
    }
}

#else
#   error Vulkan renderer is not supported on this platform
#endif
