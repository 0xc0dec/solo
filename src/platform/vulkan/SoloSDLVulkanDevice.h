#pragma once

#include "../sdl/SoloSDLDevice.h"
#include <vulkan.h>


namespace solo
{
    class SDLVulkanDevice final: public SDLDevice
    {
    public:
        virtual void saveScreenshot(const std::string& path) override final;
        virtual auto getCanvasSize() const -> Vector2 override final;

    protected:
        virtual void endUpdate() override final;

    private:
        friend class Device;

        explicit SDLVulkanDevice(const DeviceCreationArgs& args);

        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
        VkDevice device;
        VkQueue queue;
    };
}