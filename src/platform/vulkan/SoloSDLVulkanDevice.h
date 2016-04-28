#pragma once

#include "SoloDevice.h"
#include <vulkan.h>


namespace solo
{
    class VulkanDevice final: public Device
    {
    public:
        virtual void setWindowTitle(const std::string& title) override final;
        virtual auto getWindowTitle() const -> std::string override final;
        virtual void saveScreenshot(const std::string& path) override final;
        virtual void setCursorCaptured(bool captured) override final;
        virtual auto getCanvasSize() const -> Vector2 override final;
        virtual auto getLifetime() const -> float override final;
    
    private:
        friend class Device;

        explicit VulkanDevice(const DeviceCreationArgs& args);

        virtual void beginUpdate() override final;
        virtual void endUpdate() override final;

        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
        VkDevice device;
        VkQueue queue;
    };
}