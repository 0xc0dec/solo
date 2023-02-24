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
#include "SoloVulkanDriverDevice.h"
#include "SoloVulkanPipelineContext.h"

namespace solo {
    class Device;
    class Camera;
    class VulkanDevice;
    class VulkanMesh;
    class VulkanMaterial;
    class VulkanDebugInterface;

    class VulkanRenderer final: public Renderer {
    public:
        explicit VulkanRenderer(Device *device);
        ~VulkanRenderer() = default;

        void beginCamera(Camera *camera) override;
        void endCamera(Camera *camera) override;
        void renderMesh(Mesh *mesh, Transform *transform, Material *material) override;
        void renderMeshIndex(Mesh *mesh, u32 index, Transform *transform, Material *material) override;
        void renderDebugInterface(DebugInterface *debugInterface) override;

        auto name() const -> const char *override {
            return "Vulkan";
        }
        auto gpuName() const -> const char *override {
            return driverDevice_.gpuName();
        }

        auto device() const -> const VulkanDriverDevice & {
            return driverDevice_;
        }
        auto swapchain() -> VulkanSwapchain & { return swapchain_; }

    private:
        struct RenderPassContext {
            VulkanResource<VkSemaphore> completeSemaphore;
            VulkanCmdBuffer cmdBuf;
            u32 frameOfLastUse = 0;
        };

        VulkanDevice *device_ = nullptr;
        VulkanDriverDevice driverDevice_;
        VulkanSwapchain swapchain_;
        u32 frameNr_ = 0;
        umap<VulkanRenderPass *, RenderPassContext> renderPassContexts_;
        umap<size_t, VulkanPipelineContext> pipelineContexts_;

        struct {
            Camera *camera = nullptr;
            VulkanRenderPass *renderPass = nullptr;
            VulkanCmdBuffer *cmdBuffer = nullptr;
            VkSemaphore waitSemaphore = nullptr;
            size_t pipelineContextKey = 0;

            struct {
                VulkanDebugInterface *instance = nullptr;
                VulkanCmdBuffer renderCmdBuffer;
                VulkanResource<VkSemaphore> completeSemaphore;
            } debugInterface;
        } context_;

        void beginFrame() override;
        void endFrame() override;
        void bindPipelineAndMesh(Material *material, Transform *transform, Mesh *mesh);
        void cleanupUnusedRenderPassContexts();
        void cleanupUnusedPipelineContexts();
    };
}

#endif
