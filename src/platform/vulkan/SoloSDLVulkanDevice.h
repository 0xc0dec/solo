#pragma once

#include "../sdl/SoloSDLDevice.h"
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan.h>


namespace solo
{
    class SDLVulkanDevice final: public SDLDevice
    {
    public:
        virtual ~SDLVulkanDevice();

        virtual void saveScreenshot(const std::string& path) override final;
        virtual auto getCanvasSize() const -> Vector2 override final;

    protected:
        virtual void endUpdate() override final;

    private:
        friend class Device;

        explicit SDLVulkanDevice(const DeviceCreationArgs& args);

        bool selectDepthFormat();

        VkInstance instance = nullptr;
        VkPhysicalDevice physicalDevice = nullptr;
        VkDevice device = nullptr;
        VkQueue queue = nullptr;
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
        VkFormat depthFormat;
        VkSemaphore presentCompleteSem;
        VkSemaphore renderCompleteSem;
        VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        VkSubmitInfo submitInfo;
    };
}