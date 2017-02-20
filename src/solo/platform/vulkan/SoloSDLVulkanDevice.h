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

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "../sdl/SoloSDLDevice.h"
#include "SoloVulkan.h"

namespace solo
{
    namespace vk
    {
        class SDLVulkanDevice final : public SDLDevice
        {
        public:
            explicit SDLVulkanDevice(const DeviceSetup &setup);
            ~SDLVulkanDevice();

            auto getCanvasSize() const->Vector2 override final;

            void saveScreenshot(const std::string &path) override final;

            auto getVkInstance() const->VkInstance;
            auto getVkSurface() const->VkSurfaceKHR;

        protected:
            void endUpdate() override final;

        private:
            Resource<VkInstance> instance;
            Resource<VkSurfaceKHR> surface;
            VkDebugReportCallbackEXT debugCallback = nullptr;
        };

        inline auto SDLVulkanDevice::getVkInstance() const -> VkInstance
        {
            return instance;
        }

        inline auto SDLVulkanDevice::getVkSurface() const -> VkSurfaceKHR
        {
            return surface;
        }
    }
}

#endif
