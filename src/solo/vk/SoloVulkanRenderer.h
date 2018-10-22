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
        ~VulkanRenderer() = default;

        void beginCamera(Camera *camera, FrameBuffer *renderTarget) override final;
        void endCamera(Camera *camera, FrameBuffer *renderTarget) override final;
        void drawMesh(Mesh *mesh, Transform *transform, Material *material) override final;
        void drawMeshPart(Mesh *mesh, u32 part, Transform *transform, Material *material) override final;

        // TODO avoid these?
        auto device() const -> VkDevice { return device_; }
        auto physicalDevice() const -> VkPhysicalDevice { return physicalDevice_; }
        auto physicalFeatures() const -> VkPhysicalDeviceFeatures { return physicalFeatures_; }
        auto physicalProperties() const -> VkPhysicalDeviceProperties { return physicalProperties_; }
        auto physicalMemoryFeatures() const -> VkPhysicalDeviceMemoryProperties { return physicalMemoryFeatures_; }
        auto colorFormat() const -> VkFormat { return colorFormat_; }
        auto depthFormat() const -> VkFormat { return depthFormat_; }
        auto colorSpace() const -> VkColorSpaceKHR { return colorSpace_; }
        auto commandPool() const -> VkCommandPool { return commandPool_; }
        auto queue() const -> VkQueue { return queue_; }

    protected:
        void beginFrame() override final;
        void endFrame() override final;

    private:
        Device *engineDevice_ = nullptr;

        VulkanResource<VkDevice> device_;
        VulkanResource<VkCommandPool> commandPool_;
        VkPhysicalDevice physicalDevice_ = nullptr;
        VkPhysicalDeviceFeatures physicalFeatures_{};
        VkPhysicalDeviceProperties physicalProperties_{};
        VkPhysicalDeviceMemoryProperties physicalMemoryFeatures_{};
        VkFormat colorFormat_ = VK_FORMAT_UNDEFINED;
        VkFormat depthFormat_ = VK_FORMAT_UNDEFINED;
        VkColorSpaceKHR colorSpace_ = VK_COLOR_SPACE_MAX_ENUM_KHR;
        VkQueue queue_ = nullptr;
        VulkanResource<VkDebugReportCallbackEXT> debugCallback_;
        VulkanSwapchain swapchain_;

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

        u32 frame_ = 0;

        umap<VulkanRenderPass*, RenderPassContext> renderPassContexts_;
        umap<size_t, PipelineContext> pipelineContexts_;

        Camera *currentCamera_ = nullptr;
        VulkanRenderPass *currentRenderPass_ = nullptr;
        VkCommandBuffer currentCmdBuffer_ = nullptr;
        VkSemaphore prevSemaphore_ = nullptr;

        void prepareAndBindMesh(Material *material, Transform *transform, Mesh *mesh);
        auto ensurePipelineContext(Transform *transform, VulkanMaterial *material, VulkanMesh *mesh) -> PipelineContext&;
        void cleanupUnusedRenderPassContexts();
        void cleanupUnusedPipelineContexts();
    };
}

#endif
