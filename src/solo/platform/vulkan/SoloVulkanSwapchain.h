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

#include "SoloVulkan.h"
#include <vector>

namespace solo
{
    class VulkanSwapchain
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(VulkanSwapchain)

        VulkanSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkRenderPass renderPass,
            VkImageView depthStencilView, uint32_t width, uint32_t height, bool vsync, VkFormat colorFormat, VkColorSpaceKHR colorSpace);
        ~VulkanSwapchain();

        auto getHandle() const->VkSwapchainKHR const&;
        auto getSegmentCount() const->uint32_t;
        auto getImageView(uint32_t idx)->VkImageView;
        auto getFramebuffer(uint32_t idx) const->VkFramebuffer;

        auto getNextImageIndex(VkSemaphore semaphore) const->uint32_t;

    private:
        struct SwapchainStep
        {
            VkImage image = nullptr;
            VkImageView imageView = nullptr;
            VkFramebuffer framebuffer = nullptr;
        };

        VkDevice device = nullptr;
        VkSwapchainKHR swapchain = nullptr;
        std::vector<SwapchainStep> steps;
    };

    inline auto VulkanSwapchain::getHandle() const -> VkSwapchainKHR const&
    {
        return swapchain;
    }

    inline auto VulkanSwapchain::getSegmentCount() const -> uint32_t
    {
        return steps.size();
    }

    inline auto VulkanSwapchain::getImageView(uint32_t idx) -> VkImageView
    {
        return steps[idx].imageView;
    }

    inline auto VulkanSwapchain::getFramebuffer(uint32_t idx) const->VkFramebuffer
    {
        return steps[idx].framebuffer;
    }
}


#endif