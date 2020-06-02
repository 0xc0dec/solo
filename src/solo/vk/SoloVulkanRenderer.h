/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloRenderer.h"
#include "SoloVulkanSwapchain.h"
#include "SoloVulkan.h"
#include "SoloVulkanCmdBuffer.h"
#include "SoloVulkanDevice.h"
#include "SoloVulkanPipelineContext.h"

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

        void beginCamera(Camera *camera) override;
        void endCamera(Camera *camera) override;
        void renderMesh(Mesh *mesh, Transform *transform, Material *material) override;
        void renderMeshIndex(Mesh *mesh, u32 index, Transform *transform, Material *material) override;

        auto name() const -> const char* override { return "Vulkan"; }
        auto gpuName() const -> const char* override { return device_.gpuName(); }

        auto device() const -> const VulkanDevice& { return device_; }
    	auto swapchain() -> VulkanSwapchain& { return swapchain_; }

    	// TODO remove
    	auto currentCmdBuffer() const -> const VulkanCmdBuffer& { return *currentCmdBuffer_; }

    protected:
        void beginFrame() override;
        void endFrame() override;

    private:
        Device *engineDevice_ = nullptr;
    	
        VulkanDevice device_;
        VulkanSwapchain swapchain_;

        struct RenderPassContext
        {
            VulkanResource<VkSemaphore> completeSemaphore;
            VulkanCmdBuffer cmdBuf;
            u32 frameOfLastUse = 0;
        };

        u32 frame_ = 0;

        umap<VulkanRenderPass*, RenderPassContext> renderPassContexts_;
    	umap<size_t, VulkanPipelineContext> pipelineContexts_;

    	// TODO Introduce "frame" struct serving as state
        Camera *currentCamera_ = nullptr;
        VulkanRenderPass *currentRenderPass_ = nullptr;
        VulkanCmdBuffer *currentCmdBuffer_ = nullptr;
        VkSemaphore prevSemaphore_ = nullptr;
        size_t currentPipelineContextKey_ = 0;

        void bindPipelineAndMesh(Material *material, Transform *transform, Mesh *mesh);
        void cleanupUnusedRenderPassContexts();
        void cleanupUnusedPipelineContexts();
    };
}

#endif
