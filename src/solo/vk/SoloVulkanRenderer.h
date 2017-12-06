/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

// NOTE This file, as well as other vulkan files in the engine, is a total hackfest and test playground currently.
// Don't pay attention to the quality of code here.

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloRenderer.h"
#include "SoloVulkanSwapchain.h"
#include "SoloVulkanPipeline.h"
#include "SoloRenderCommand.h"
#include "SoloVulkan.h"
#include "SoloVulkanDescriptorPool.h"
#include "SoloVulkanBuffer.h"

namespace solo
{
    class Device;
    class Camera;
    class VulkanMesh;

    class VulkanRenderer final : public Renderer
    {
    public:
        explicit VulkanRenderer(Device *device);
        ~VulkanRenderer();

        // TODO avoid these?
        auto getDevice() const -> VkDevice { return device; }
        auto getPhysicalDevice() const -> VkPhysicalDevice { return physicalDevice; }
        auto getPhysicalFeatures() const -> VkPhysicalDeviceFeatures { return physicalFeatures; }
        auto getPhysicalProperties() const -> VkPhysicalDeviceProperties { return physicalProperties; }
        auto getPhysicalMemoryFeatures() const -> VkPhysicalDeviceMemoryProperties { return physicalMemoryFeatures; }
        auto getColorFormat() const -> VkFormat { return colorFormat; }
        auto getDepthFormat() const -> VkFormat { return depthFormat; }
        auto getColorSpace() const -> VkColorSpaceKHR { return colorSpace; }
        auto getCommandPool() const -> VkCommandPool { return commandPool; }
        auto getQueue() const -> VkQueue { return queue; }

        void addRenderCommand(const RenderCommand &cmd) override final { renderCommands.push_back(cmd); }

    protected:
        void beginFrame() override final;
        void endFrame() override final;

    private:
        Device *engineDevice = nullptr;

        VulkanResource<VkDevice> device;
        VulkanResource<VkCommandPool> commandPool;
        VkPhysicalDevice physicalDevice = nullptr;
        VkPhysicalDeviceFeatures physicalFeatures{};
        VkPhysicalDeviceProperties physicalProperties{};
        VkPhysicalDeviceMemoryProperties physicalMemoryFeatures{};
        VkFormat colorFormat = VK_FORMAT_UNDEFINED;
        VkFormat depthFormat = VK_FORMAT_UNDEFINED;
        VkColorSpaceKHR colorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR;
        VkQueue queue = nullptr;
        VulkanResource<VkDebugReportCallbackEXT> debugCallback;
        VulkanSwapchain swapchain;

        vec<RenderCommand> renderCommands;
        vec<VulkanPipeline> pipelines;

        struct NodeBinding
        {
            umap<str, VulkanBuffer> buffers;
            VulkanDescriptorPool descPool;
            VulkanResource<VkDescriptorSetLayout> descSetLayout;
            VkDescriptorSet descSet = VK_NULL_HANDLE;
        };

        struct
        {
            bool initialized = false;
//            VulkanImage colorAttachment;
//            VulkanImage depthAttachment;
//            VulkanRenderPass renderPass;
//            VulkanResource<VkFramebuffer> frameBuffer;
        } test;

        struct RenderPassContext
        {
            bool started = false;
            VulkanResource<VkSemaphore> completeSemaphore;
            VulkanResource<VkCommandBuffer> cmdBuffer;
            VulkanRenderPass *renderPass = nullptr;
        };

        umap<VulkanRenderPass*, RenderPassContext> renderPassContexts;

        // TODO clear this when bindings get no longer used
        umap<const Material*, umap<const Transform*, umap<const Camera*, NodeBinding>>> nodeMaterialBindings;

        umap<const Camera*, VulkanResource<VkCommandBuffer>> cameraCmdBuffers;

        void drawMeshPart(Material *mat, Transform *transform, Mesh *m, const Camera *currentCamera,
            u32 part, VkCommandBuffer cmdBuf, VkRenderPass renderPass);
    };
}

#endif
