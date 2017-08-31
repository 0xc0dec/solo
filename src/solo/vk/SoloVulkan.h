/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanResource.h"

#include <vector>
#include <functional>

#ifdef SL_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan.h>

#ifdef SL_ERR_CHECK
#   define SL_VK_CHECK_RESULT(vkCall, ...) SL_PANIC_IF(vkCall != VK_SUCCESS, __VA_ARGS__)
#else
#   define SL_VK_CHECK_RESULT(vkCall) vkCall
#endif

namespace solo
{
    namespace vk
    {
        struct DepthStencil
        {
            Resource<VkImage> image;
            Resource<VkDeviceMemory> mem;
            Resource<VkImageView> view;
        };

        auto getPhysicalDevice(VkInstance instance) -> VkPhysicalDevice;
        auto createDevice(VkPhysicalDevice physicalDevice, uint32_t queueIndex) -> Resource<VkDevice>;
        auto createSemaphore(VkDevice device) -> Resource<VkSemaphore>;
        auto getDepthFormat(VkPhysicalDevice device) -> VkFormat;
        void submitCommandBuffer(VkQueue queue, VkCommandBuffer buffer);
        void createCommandBuffers(VkDevice device, VkCommandPool commandPool, bool primary, uint32_t count, VkCommandBuffer *result);
        auto createCommandBuffer(VkDevice device, VkCommandPool commandPool) -> Resource<VkCommandBuffer>;
        void beginCommandBuffer(VkCommandBuffer buffer, bool oneTime);
        void queueSubmit(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore *waitSemaphores,
            uint32_t signalSemaphoreCount, const VkSemaphore *signalSemaphores,
            uint32_t commandBufferCount, const VkCommandBuffer *commandBuffers);
        auto findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, uint32_t typeBits,
            VkMemoryPropertyFlags properties) -> int32_t;
        auto createRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat) -> Resource<VkRenderPass>;
        auto createFrameBuffer(VkDevice device, VkImageView colorAttachment, VkImageView depthAttachment,
            VkRenderPass renderPass, uint32_t width, uint32_t height) -> Resource<VkFramebuffer>;
        auto createShader(VkDevice device, const void *data, uint32_t size) -> Resource<VkShaderModule>;
        auto createShaderStageInfo(bool vertex, VkShaderModule shader, const char *entryPoint) -> VkPipelineShaderStageCreateInfo;
        auto createDepthStencil(VkDevice device, VkPhysicalDeviceMemoryProperties physicalDeviceMemProps,
            VkFormat depthFormat, uint32_t canvasWidth, uint32_t canvasHeight) -> DepthStencil;
        auto createDebugCallback(VkInstance instance, PFN_vkDebugReportCallbackEXT callbackFunc) -> Resource<VkDebugReportCallbackEXT>;
    }
}

#endif
