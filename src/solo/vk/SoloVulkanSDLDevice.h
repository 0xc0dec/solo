/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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

        auto instance() const -> VkInstance { return instance_; }
        auto surface() const -> VkSurfaceKHR { return surface_; }

    protected:
        void endUpdate() override;

    private:
        VulkanResource<VkInstance> instance_;
        VulkanResource<VkSurfaceKHR> surface_;
    };
}

#endif
