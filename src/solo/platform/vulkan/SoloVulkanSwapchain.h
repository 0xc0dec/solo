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
#include "SoloVulkan.h"
#include "SoloVector2.h"

#ifdef SL_VULKAN_RENDERER

namespace solo
{
    class VulkanSwapchain
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(VulkanSwapchain)

        VulkanSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
            bool vsync, const Vector2 &deviceCanvasSize, VkFormat colorFormat, VkColorSpaceKHR colorSpace);
        ~VulkanSwapchain();

        auto getImageCount() const -> uint32_t;
        auto getImageView(uint32_t index) const -> VkImageView;

        auto acquireNextImage(VkSemaphore presentCompleteSem) const -> uint32_t;
        void present(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSem) const;

    private:
        struct SwapchainBuffer
        {
            VkImage image = nullptr;
            VkImageView imageView = nullptr;
        };

        VkDevice device = nullptr;
        VkSwapchainKHR swapchain = nullptr;
        uint32_t canvasWidth = 1;
        uint32_t canvasHeight = 1;
        std::vector<SwapchainBuffer> buffers;
    };

    inline auto VulkanSwapchain::getImageCount() const -> uint32_t
    {
        return buffers.size();
    }

    inline auto VulkanSwapchain::getImageView(uint32_t index) const -> VkImageView
    {
        return buffers[index].imageView;
    }
}

#endif