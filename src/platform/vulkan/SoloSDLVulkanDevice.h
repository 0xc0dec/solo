// Note: this file is a total hackery and a work-in-progress, so avoid reading to stay sane

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