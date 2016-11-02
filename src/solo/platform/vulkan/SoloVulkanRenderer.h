#pragma once

#include "SoloRenderer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"

namespace solo
{
    class Device;
    class Vector2;

    class VulkanRenderer final: public Renderer
    {
    public:
        explicit VulkanRenderer(Device* device);
        ~VulkanRenderer();

        void beginFrame() override final;
        void endFrame() override final;

    private:
        struct SwapchainBuffer
        {
            VkImage image = nullptr;
            VkImageView imageView = nullptr;
        };

        struct DepthStencil
        {
            VkImage image;
		    VkDeviceMemory mem;
		    VkImageView view;
        } depthStencil;

        void initSwapchain(VkSurfaceKHR surface, bool vsync, const Vector2& deviceCanvasSize);
        void initCommandBuffers();
        void initPresentationCommandBuffers();
        void beginCommandBuffer(VkCommandBuffer buffer);
        auto createDepthStencil(VkDevice device, VkPhysicalDeviceMemoryProperties physicalDeviceMemProps,
            VkCommandBuffer cmdBuffer, VkFormat depthFormat, uint32_t canvasWidth, uint32_t canvasHeight) -> DepthStencil;
        void initFrameBuffers();

        uint32_t canvasWidth = 1;
        uint32_t canvasHeight = 1;
        VkDevice device = nullptr;
        VkPhysicalDevice physicalDevice = nullptr;
        VkQueue queue = nullptr;
        VkPhysicalDeviceFeatures features;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceMemoryProperties memProperties;
        VkFormat depthFormat = VK_FORMAT_UNDEFINED;
        VkFormat colorFormat = VK_FORMAT_UNDEFINED;
        VkColorSpaceKHR colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        VkSemaphore presentCompleteSem = nullptr;
        VkSemaphore renderCompleteSem = nullptr;
        VkCommandPool commandPool = nullptr;
        VkSwapchainKHR swapchain = nullptr;
        VkRenderPass renderPass = nullptr;
        std::vector<SwapchainBuffer> swapchainBuffers;
        std::vector<VkCommandBuffer> drawCmdBuffers;
        std::vector<VkCommandBuffer> prePresentCmdBuffers;
        std::vector<VkCommandBuffer> postPresentCmdBuffers;
        std::vector<VkFramebuffer> frameBuffers;
        uint32_t currentBuffer = 0;
    };
}

#else
#   error Vulkan renderer is not supported on this platform
#endif
