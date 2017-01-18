/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include <vector>
#include <functional>

#ifdef SL_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan.h>

#ifdef SL_ERR_CHECK
#   define SL_CHECK_VK_RESULT(vkCall, ...) SL_ERR_CHECK_BLOCK(if (vkCall != VK_SUCCESS) SL_ERR(__VA_ARGS__))
#else
#   define SL_CHECK_VK_RESULT(vkCall) vkCall
#endif

namespace solo
{
    namespace vk
    {
        struct DepthStencil
        {
            VkImage image;
            VkDeviceMemory mem;
            VkImageView view;
        };

        auto createDevice(VkPhysicalDevice physicalDevice, uint32_t queueIndex) -> VkDevice;
        auto getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface) -> std::tuple<VkFormat, VkColorSpaceKHR>;
        auto createSemaphore(VkDevice device) -> VkSemaphore;
        auto getPhysicalDevice(VkInstance instance) -> VkPhysicalDevice;
        auto getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface) -> uint32_t;
        auto getDepthFormat(VkPhysicalDevice device) -> VkFormat;
        auto createCommandPool(VkDevice logicalDevice, uint32_t queueIndex) -> VkCommandPool;
        void submitCommandBuffer(VkQueue queue, VkCommandBuffer buffer);
        void createCommandBuffers(VkDevice logicalDevice, VkCommandPool commandPool, uint32_t count, VkCommandBuffer *result);
        void destroyCommandBuffers(VkDevice device, VkCommandPool commandPool, VkCommandBuffer *buffers, uint32_t count);
        auto findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, uint32_t typeBits,
            VkMemoryPropertyFlags properties) -> int32_t;
        auto createRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat) -> VkRenderPass;
        auto createFrameBuffer(VkDevice device, VkImageView colorAttachment, VkImageView depthAttachment,
            VkRenderPass renderPass, uint32_t width, uint32_t height) -> VkFramebuffer;
        auto createShader(VkDevice device, const std::vector<uint8_t>& data) -> VkShaderModule;
        auto createDebugCallback(VkInstance instance, PFN_vkDebugReportCallbackEXT callbackFunc) -> VkDebugReportCallbackEXT;
        void destroyDebugCallback(VkInstance instance, VkDebugReportCallbackEXT callback);
        auto createDepthStencil(VkDevice device, VkPhysicalDeviceMemoryProperties physicalDeviceMemProps,
            VkFormat depthFormat, uint32_t canvasWidth, uint32_t canvasHeight) -> DepthStencil;
        void recordCommandBuffer(VkCommandBuffer buffer, std::function<void(VkCommandBuffer)> action);
        auto createRasterizationStateInfo(bool depthClamp, bool discardEnabled, VkCullModeFlags cullMode, VkFrontFace frontFace)
            -> VkPipelineRasterizationStateCreateInfo;
        auto createMultisampleStateInfo(VkSampleCountFlagBits rasterizationSampleCount) -> VkPipelineMultisampleStateCreateInfo;
        auto createBlendAttachmentState(bool blendEnabled, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor,
            VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp,
            VkColorComponentFlags colorWriteMask) -> VkPipelineColorBlendAttachmentState;
        auto createColorBlendStateInfo(VkPipelineColorBlendAttachmentState* blendAttachments, bool logicOpEnabled, VkLogicOp logicOp)
            -> VkPipelineColorBlendStateCreateInfo;
        auto createShaderStageInfo(bool vertex, VkShaderModule shader, const char* entryPoint) -> VkPipelineShaderStageCreateInfo;
    }
}

#endif
