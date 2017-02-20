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

#include "SoloRenderer.h"
#include "SoloVulkanSwapchain.h"
#include "SoloVulkanRenderPass.h"
#include "SoloVulkan.h"
#include "SoloVector4.h"

namespace solo
{
    class Device;

    class VulkanRenderer final: public Renderer
    {
    public:
        explicit VulkanRenderer(Device *device);
        ~VulkanRenderer();

        void setClear(const Vector4 &color, bool clearColor, bool clearDepth);
        void setViewport(const Vector4 &viewport);

    protected:
        void beginFrame() override final;
        void endFrame() override final;

    private:
        uint32_t canvasWidth = 0;
        uint32_t canvasHeight = 0;
        VkFormat depthFormat = VK_FORMAT_UNDEFINED;
        VkFormat colorFormat = VK_FORMAT_UNDEFINED;
        VkColorSpaceKHR colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        
        struct
        {
            VkPhysicalDevice device = nullptr;
            VkPhysicalDeviceFeatures features;
            VkPhysicalDeviceProperties properties;
            VkPhysicalDeviceMemoryProperties memProperties;
        } physicalDevice;

        vk::Resource<VkDevice> device;
        VkQueue queue = nullptr;
        vk::Resource<VkCommandPool> commandPool;
        vk::DepthStencil depthStencil;
        RenderPass renderPass;
        VulkanSwapchain swapchain;

        struct
        {
            vk::Resource<VkSemaphore> renderComplete;
            vk::Resource<VkSemaphore> presentComplete;
        } semaphores;
        
        std::vector<VkCommandBuffer> cmdBuffers;
        uint32_t currentBuffer = 0;

        bool dirty = true;

        std::vector<VkClearValue> clearValues;
        VkViewport viewport = {1, 1, 0, 1};

        void updateCmdBuffers();
    };
}

#endif
