#pragma once

#include "SoloCommon.h"
#include "../sdl/SoloSDLDevice.h"

#ifdef SL_VULKAN_RENDERER

namespace solo
{
    class SDLVulkanDevice: public SDLDevice
    {
    public:
        explicit SDLVulkanDevice(const DeviceCreationArgs& args);

    private:
    };
}

#else
#   error Vulkan renderer is not supported on this platform
#endif