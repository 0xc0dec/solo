/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloRenderer.h"
#include "SoloVulkanSwapchain.h"
#include "SoloVulkanPipeline.h"
#include "SoloVulkan.h"
#include "SoloVulkanBuffer.h"
#include "SoloVulkanDescriptorSet.h"
#include "SoloVulkanCmdBuffer.h"
#include "SoloVulkanDevice.h"

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

        auto device() const -> const VulkanDevice& { return device_; }

    protected:
        void beginFrame() override final;
        void endFrame() override final;

    private:
        Device *engineDevice_ = nullptr;

        VulkanDevice device_;
        VulkanSwapchain swapchain_;

        struct PipelineContext
        {
            umap<str, VulkanBuffer> uniformBuffers;
            VulkanPipeline pipeline;
            VulkanDescriptorSet descSet;
            size_t lastMaterialFlagsHash = 0;
            size_t lastMeshLayoutHash = 0;
            u32 frameOfLastUse = 0;
        };

        struct RenderPassContext
        {
            VulkanResource<VkSemaphore> completeSemaphore;
            VulkanCmdBuffer cmdBuf;
            VulkanRenderPass *renderPass = nullptr;
            u32 frameOfLastUse = 0;
        };

        u32 frame_ = 0;

        umap<VulkanRenderPass*, RenderPassContext> renderPassContexts_;
        umap<size_t, PipelineContext> pipelineContexts_;

        Camera *currentCamera_ = nullptr;
        VulkanRenderPass *currentRenderPass_ = nullptr;
        VulkanCmdBuffer *currentCmdBuffer_ = nullptr;
        VkSemaphore prevSemaphore_ = nullptr;

        void prepareAndBindMesh(Material *material, Transform *transform, Mesh *mesh);
        auto ensurePipelineContext(Transform *transform, VulkanMaterial *material, VulkanMesh *mesh) -> PipelineContext&;
        void cleanupUnusedRenderPassContexts();
        void cleanupUnusedPipelineContexts();
    };
}

#endif
