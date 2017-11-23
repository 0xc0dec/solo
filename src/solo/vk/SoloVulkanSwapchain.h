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
    class VulkanRenderer;
    class VulkanSDLDevice;

    class VulkanSwapchain
    {
    public:
        VulkanSwapchain() {}

        VulkanSwapchain(VulkanRenderer *renderer, VulkanSDLDevice *device, u32 width, u32 height, bool vsync);
        VulkanSwapchain(const VulkanSwapchain &other) = delete;
        VulkanSwapchain(VulkanSwapchain &&other) = default;

        ~VulkanSwapchain() {}

        auto operator=(const VulkanSwapchain &other) -> VulkanSwapchain& = delete;
        auto operator=(VulkanSwapchain &&other) -> VulkanSwapchain& = default;

        operator VkSwapchainKHR() { return swapchain; }
        operator VkSwapchainKHR() const { return swapchain; }

        auto getRenderPass() -> VulkanRenderPass& { return renderPass; }
        auto getPresentCompleteSemaphore() const -> VkSemaphore { return presentCompleteSem; }

        void recordCommandBuffers(std::function<void(VkFramebuffer, VkCommandBuffer)> issueCommands);
        auto acquireNext() -> VkSemaphore;
        void presentNext(VkQueue queue, u32 waitSemaphoreCount, const VkSemaphore *waitSemaphores);

    private:
        struct Step
        {
            VkImage image;
            VulkanResource<VkImageView> imageView;
            VulkanResource<VkFramebuffer> framebuffer;
            VulkanResource<VkCommandBuffer> cmdBuffer;
        };

        VkDevice device = nullptr;
        VulkanResource<VkSwapchainKHR> swapchain;
        VulkanImage depthStencil;
        vec<Step> steps;
        VulkanResource<VkSemaphore> presentCompleteSem;
        VulkanResource<VkSemaphore> renderCompleteSem;
        VulkanRenderPass renderPass;
        u32 nextStep = 0;
    };
}


#endif
