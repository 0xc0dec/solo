#pragma once

#include "SoloCommon.h"
#include "../sdl/SoloSDLDevice.h"
#define VK_USE_PLATFORM_WIN32_KHR 
#include <vulkan.h>

#ifdef SL_VULKAN_RENDERER

namespace solo
{
    class SDLVulkanDevice final: public SDLDevice
    {
    public:
        explicit SDLVulkanDevice(const DeviceCreationArgs& args);

        void saveScreenshot(const std::string& path) override final;
        auto getCanvasSize() const -> Vector2 override final;

    protected:
        void endUpdate() override final;

    private:
        VkInstance instance = nullptr;
        VkDevice device = nullptr;
        VkQueue graphicsQueue = nullptr;
        VkSurfaceKHR surface = nullptr;
        VkFormat colorFormat = VK_FORMAT_UNDEFINED;
        VkColorSpaceKHR colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
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