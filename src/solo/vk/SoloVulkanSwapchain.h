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

    class VulkanSwapchain final
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

        auto getCurrentFrameBuffer() -> VkFramebuffer { return steps[currentStep].framebuffer; }
        auto getRenderPass() -> VulkanRenderPass& { return renderPass; }

        auto moveNext() -> VkSemaphore;
        void present(VkQueue queue, u32 waitSemaphoreCount, const VkSemaphore *waitSemaphores);

    private:
        struct Step
        {
            VkImage image = nullptr;
            VulkanResource<VkImageView> imageView;
            VulkanResource<VkFramebuffer> framebuffer;
            VulkanResource<VkCommandBuffer> cmdBuffer;
        };

        VkDevice device = nullptr;
        VulkanResource<VkSwapchainKHR> swapchain;
        VulkanImage depthStencil;
        vec<Step> steps;
        VulkanResource<VkSemaphore> presentCompleteSem;
        VulkanRenderPass renderPass;
        u32 currentStep = 0;
    };
}


#endif
