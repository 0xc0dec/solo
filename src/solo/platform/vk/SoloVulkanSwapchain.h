/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"
#include <vector>

namespace solo
{
    namespace vk
    {
        class Swapchain
        {
        public:
            Swapchain();
            Swapchain(const Swapchain &other) = delete;
            Swapchain(Swapchain &&other) noexcept;
            Swapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkRenderPass renderPass,
                VkImageView depthStencilView, uint32_t width, uint32_t height, bool vsync, VkFormat colorFormat, VkColorSpaceKHR colorSpace);
            ~Swapchain() {}

            auto operator=(Swapchain other) noexcept -> Swapchain&;

            auto getNextImageIndex(VkSemaphore semaphore) const -> uint32_t;
            auto getStepCount() const -> uint32_t;
            auto getFramebuffer(uint32_t idx) const -> VkFramebuffer;
            auto getImageView(uint32_t idx) -> VkImageView;

            auto getHandle() const -> VkSwapchainKHR;

        private:
            struct Step
            {
                VkImage image;
                Resource<VkImageView> imageView;
                Resource<VkFramebuffer> framebuffer;
            };

            VkDevice device = nullptr;
            Resource<VkSwapchainKHR> swapchain;
            std::vector<Step> steps;

            void swap(Swapchain &other) noexcept;
        };

        inline auto Swapchain::getHandle() const -> VkSwapchainKHR
        {
            return swapchain;
        }

        inline auto Swapchain::getStepCount() const -> uint32_t
        {
            return static_cast<uint32_t>(steps.size());
        }

        inline auto Swapchain::getFramebuffer(uint32_t idx) const -> VkFramebuffer
        {
            return steps[idx].framebuffer;
        }

        inline auto Swapchain::getImageView(uint32_t idx) -> VkImageView
        {
            return steps[idx].imageView;
        }
    }
}


#endif