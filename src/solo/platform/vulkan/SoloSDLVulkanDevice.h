#pragma once

#include "../sdl/SoloSDLDevice.h"

#ifdef SL_VULKAN_RENDERER

namespace solo
{
    class SDLVulkanDevice final: public SDLDevice
    {
    public:
        explicit SDLVulkanDevice(const DeviceSetup& setup);

        auto getCanvasSize() const -> Vector2 override final;
        void saveScreenshot(const std::string& path) override final;

    protected:
        void endUpdate() override final;
    };
}

#else
#   error Vulkan renderer is not supported on this platform
#endif
