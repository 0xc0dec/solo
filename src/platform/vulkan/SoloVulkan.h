#pragma once

#include "SoloCommon.h"
#include <vector>

#ifdef SL_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR 
#endif
#include <vulkan.h>

#ifdef SL_ERR_CHECK
#   define SL_CHECK_VK_RESULT(vkCall, ...) SL_DBG_BLOCK(if (vkCall != VK_SUCCESS) SL_ERR(__VA_ARGS__))
#else
#   define SL_CHECK_VK_RESULT(vkCall) vkCall
#endif

namespace solo
{
    struct VulkanHelper
    {
        static auto createShader(VkDevice logicalDevice, const std::vector<uint8_t>& data) -> VkShaderModule;

        static auto findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
            uint32_t typeBits, VkMemoryPropertyFlags properties) -> int32_t;

        static auto createLogicalDevice(VkPhysicalDevice physicalDevice, uint32_t queueIndex) -> VkDevice;
        static auto createPhysicalDevice(VkInstance instance) -> VkPhysicalDevice;

        static auto getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface) -> uint32_t;

        static void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldLayout,
            VkImageLayout newLayout, VkImageSubresourceRange subresourceRange);
        static void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask,
            VkImageLayout oldLayout, VkImageLayout newLayout);

        static auto createFrameBuffer(VkDevice logicalDevice, VkImageView colorAttachment, VkImageView depthAttachment,
            VkRenderPass renderPass, uint32_t width, uint32_t height) -> VkFramebuffer;

        static auto createRenderPass(VkDevice logicalDevice, VkFormat colorFormat, VkFormat depthFormat) -> VkRenderPass;

        static auto createCommandPool(VkDevice logicalDevice, uint32_t queueIndex) -> VkCommandPool;

        static auto createCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool) -> VkCommandBuffer;
        static void flushCommandBuffer(VkQueue queue, VkCommandBuffer buffer);

        static auto getDepthFormat(VkPhysicalDevice device) -> VkFormat;
    };
}