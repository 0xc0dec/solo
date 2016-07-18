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
        VkInstance instance;
    };
}

#else
#   error Vulkan renderer is not supported on this platform
#endif