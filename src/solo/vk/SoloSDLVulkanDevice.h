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
        class SDLDevice final : public sdl::Device
        {
        public:
            explicit SDLDevice(const DeviceSetup &setup);
            ~SDLDevice();

            void saveScreenshot(const std::string &path) override final;

            auto getInstance() const -> VkInstance;
            auto getSurface() const -> VkSurfaceKHR;

        protected:
            void endUpdate() override final;

        private:
            Resource<VkInstance> instance;
            Resource<VkSurfaceKHR> surface;
        };

        inline auto SDLDevice::getInstance() const -> VkInstance
        {
            return instance;
        }

        inline auto SDLDevice::getSurface() const -> VkSurfaceKHR
        {
            return surface;
        }
    }
}

#endif
