// Note: this file is a total hackery and a work-in-progress, so avoid reading to stay sane

#pragma once

#include "../sdl/SoloSDLDevice.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"

namespace solo
{
    class SDLVulkanDevice final: public SDLDevice
    {
    public:
        explicit SDLVulkanDevice(const DeviceSetup& setup);
        ~SDLVulkanDevice();

        void saveScreenshot(const std::string& path) override final;
        auto getCanvasSize() const -> Vector2 override final;

        auto getVulkanInstance() const -> VkInstance;
        auto getVulkanSurface() const -> VkSurfaceKHR;

    protected:
        void endUpdate() override final;

    private:
        VkInstance instance = nullptr;
        VkSurfaceKHR surface = nullptr;
    };

    inline auto SDLVulkanDevice::getVulkanInstance() const -> VkInstance
    {
        return instance;
    }

    inline auto SDLVulkanDevice::getVulkanSurface() const -> VkSurfaceKHR
    {
        return surface;
    }
}

#else
#   error Vulkan renderer is not supported on this platform
#endif