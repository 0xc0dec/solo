/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanBuffer.h"
#include "SoloVulkanPipeline.h"
#include "SoloVulkanDescriptorSet.h"

namespace solo {
    class VulkanMaterial;
    class VulkanMesh;
    class VulkanRenderPass;
    class Camera;
    class Transform;

    class VulkanPipelineContext {
    public:
        VulkanPipelineContext(VulkanDriverDevice *device, size_t key);
        VulkanPipelineContext(const VulkanPipelineContext &other) = delete;
        VulkanPipelineContext(VulkanPipelineContext &&other) noexcept;

        auto operator=(const VulkanPipelineContext &other) -> VulkanPipelineContext & = delete;
        auto operator=(VulkanPipelineContext &&other) noexcept -> VulkanPipelineContext & = delete;

        auto frameOfLastUse() const -> u32 {
            return frameOfLastUse_;
        }
        void setFrameOfLastUse(u32 frame) {
            frameOfLastUse_ = frame;
        }

        auto key() const -> size_t {
            return key_;
        }

        auto pipeline() const -> const VulkanPipeline & {
            return pipeline_;
        }
        auto descriptorSet() const -> const VulkanDescriptorSet & {
            return descSet_;
        }

        void update(VulkanMaterial *material, VulkanMesh *mesh, VulkanRenderPass *renderPass,
                    Camera *camera, Transform *transform);

    private:
        VulkanDriverDevice *device_ = nullptr;
        umap<str, VulkanBuffer> uniformBuffers_;
        VulkanPipeline pipeline_;
        VulkanDescriptorSet descSet_;
        size_t lastMaterialStateHash_ = 0;
        size_t lastMeshLayoutHash_ = 0;
        size_t key_ = 0;
        u32 frameOfLastUse_ = 0;
    };
}

#endif
