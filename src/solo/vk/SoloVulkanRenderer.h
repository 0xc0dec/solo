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
#include "SoloVulkan.h"
#include "SoloVulkanDescriptorPool.h"
#include "SoloVulkanBuffer.h"

namespace solo
{
    class Device;
    class Camera;
    class VulkanMesh;
    class VulkanMaterial;

    class VulkanRenderer final : public Renderer
    {
    public:
        explicit VulkanRenderer(Device *device);
        ~VulkanRenderer() {}

        void beginCamera(Camera *camera, FrameBuffer *renderTarget) override final;
        void endCamera(Camera *camera, FrameBuffer *renderTarget) override final;
        void drawMesh(Mesh *mesh, Transform *transform, Material *material) override final;
        void drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material) override final;

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

        struct PipelineContext
        {
            umap<str, VulkanBuffer> uniformBuffers;
            VulkanDescriptorPool descPool;
            VulkanPipeline pipeline;
            VulkanResource<VkDescriptorSetLayout> descSetLayout;
            VkDescriptorSet descSet = VK_NULL_HANDLE;
            size_t lastMaterialFlagsHash = 0;
            size_t lastMeshLayoutHash = 0;
            u32 frameOfLastUse = 0;
        };

        struct RenderPassContext
        {
            VulkanResource<VkSemaphore> completeSemaphore;
            VulkanResource<VkCommandBuffer> cmdBuffer;
            VulkanRenderPass *renderPass = nullptr;
            u32 frameOfLastUse = 0;
        };

        u32 frame = 0;

        umap<VulkanRenderPass*, RenderPassContext> renderPassContexts;
        umap<size_t, PipelineContext> pipelineContexts;

        Camera *currentCamera = nullptr;
        VulkanRenderPass *currentRenderPass = nullptr;
        VkCommandBuffer currentCmdBuffer = nullptr;
        VkSemaphore prevSemaphore = nullptr;

        void prepareAndBindMesh(VkCommandBuffer cmdBuf, VkRenderPass renderPass, Material *material,
            Transform *transform, Mesh *mesh, Camera *camera);
        void drawMeshPart(VkCommandBuffer cmdBuf, VkRenderPass renderPass, Material *material, Transform *transform,
            Mesh *mesh, Camera *camera, u32 part);
        void drawMesh(VkCommandBuffer cmdBuf, VkRenderPass renderPass, Material *material,
            Transform *transform, Mesh *mesh, Camera *camera);
        auto ensurePipelineContext(Transform *transform, Camera *camera, VulkanMaterial *material,
            VulkanMesh *mesh, VkRenderPass renderPass) -> PipelineContext&;
        void cleanupUnusedRenderPassContexts();
        void cleanupUnusedPipelineContexts();
    };
}

#endif
