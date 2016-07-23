#pragma once

#include "SoloCommon.h"
#include "../sdl/SoloSDLDevice.h"
#define VK_USE_PLATFORM_WIN32_KHR 
#include <vulkan.h>

#ifdef SL_VULKAN_RENDERER

namespace solo
{
    class SDLVulkanDevice: public SDLDevice
    {
    public:
        explicit SDLVulkanDevice(const DeviceCreationArgs& args);

    private:
        VkInstance instance = nullptr;
        VkDevice device = nullptr;
        VkQueue graphicsQueue = nullptr;
        VkPhysicalDevice physicalDevice = nullptr;
        VkPhysicalDeviceProperties physicalDeviceProperties;
        VkPhysicalDeviceFeatures physicalDeviceFeatures;
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
        VkFormat depthFormat = VK_FORMAT_UNDEFINED;
        VkSemaphore presentCompleteSem = nullptr;
        VkSemaphore renderCompleteSem = nullptr;
        VkSubmitInfo submitInfo;
        VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    };
}

#else
#   error Vulkan renderer is not supported on this platform
#endif