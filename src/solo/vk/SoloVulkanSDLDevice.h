/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "../sdl/SoloSDLDevice.h"
#include "SoloVulkan.h"

namespace solo
{
    class VulkanSDLDevice final : public SDLDevice
    {
    public:
        explicit VulkanSDLDevice(const DeviceSetup &setup);
        ~VulkanSDLDevice();

        void saveScreenshot(const str &path) override final;

        auto getInstance() const -> VkInstance { return instance; }
        auto getSurface() const -> VkSurfaceKHR { return surface; }

    protected:
        void endUpdate() override final;

    private:
        VulkanResource<VkInstance> instance;
        VulkanResource<VkSurfaceKHR> surface;
    };
}

#endif
