/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
        VulkanSwapchain() = default;
        VulkanSwapchain(VulkanRenderer *renderer, VulkanSDLDevice *device, u32 width, u32 height, bool vsync);
        VulkanSwapchain(const VulkanSwapchain &other) = delete;
        VulkanSwapchain(VulkanSwapchain &&other) = default;
        ~VulkanSwapchain() = default;

        auto operator=(const VulkanSwapchain &other) -> VulkanSwapchain& = delete;
        auto operator=(VulkanSwapchain &&other) -> VulkanSwapchain& = default;

        operator VkSwapchainKHR() { return swapchain_; }
        operator VkSwapchainKHR() const { return swapchain_; }

        auto currentFrameBuffer() -> VkFramebuffer { return steps_[currentStep_].framebuffer; }
        auto renderPass() -> VulkanRenderPass& { return renderPass_; }

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

        VkDevice device_ = nullptr;
        VulkanResource<VkSwapchainKHR> swapchain_;
        VulkanImage depthStencil_;
        vec<Step> steps_;
        VulkanResource<VkSemaphore> presentCompleteSem_;
        VulkanRenderPass renderPass_;
        u32 currentStep_ = 0;
    };
}


#endif
