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
    namespace vk
    {
        class SDLDevice final : public sdl::SDLDevice
        {
        public:
            explicit SDLDevice(const DeviceSetup &setup);
            ~SDLDevice();

            void saveScreenshot(const str &path) override final;

            auto getInstance() const -> VkInstance { return instance; }
            auto getSurface() const -> VkSurfaceKHR { return surface; }

        protected:
            void endUpdate() override final;

        private:
            Resource<VkInstance> instance;
            Resource<VkSurfaceKHR> surface;
        };
    }
}

#endif
