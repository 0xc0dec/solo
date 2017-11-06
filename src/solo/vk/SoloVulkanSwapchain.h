/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"
#include "SoloVulkanImage.h"
#include "SoloVulkanRenderPass.h"

namespace solo
{
    namespace vk
    {
        class Renderer;
        class SDLDevice;

        class Swapchain
        {
        public:
            Swapchain() {}

            Swapchain(Renderer *renderer, SDLDevice *device, u32 width, u32 height, bool vsync);
            Swapchain(const Swapchain &other) = delete;
            Swapchain(Swapchain &&other) = default;

            ~Swapchain() {}

            auto operator=(const Swapchain &other) -> Swapchain& = delete;
            auto operator=(Swapchain &&other) -> Swapchain& = default;

            operator VkSwapchainKHR() { return swapchain; }
            operator VkSwapchainKHR() const { return swapchain; }

            auto getRenderPass() -> RenderPass& { return renderPass; }
            auto getPresentCompleteSemaphore() const -> VkSemaphore { return presentCompleteSem; }

            void recordCommandBuffers(std::function<void(VkFramebuffer, VkCommandBuffer)> issueCommands);
            auto acquireNext() -> VkSemaphore;
            void presentNext(VkQueue queue, u32 waitSemaphoreCount, const VkSemaphore *waitSemaphores);

        private:
            struct Step
            {
                VkImage image;
                Resource<VkImageView> imageView;
                Resource<VkFramebuffer> framebuffer;
                Resource<VkCommandBuffer> cmdBuffer;
            };

            VkDevice device = nullptr;
            Resource<VkSwapchainKHR> swapchain;
            Image depthStencil;
            vec<Step> steps;
            Resource<VkSemaphore> presentCompleteSem;
            Resource<VkSemaphore> renderCompleteSem;
            RenderPass renderPass;
            u32 nextStep = 0;
        };
    }
}


#endif
