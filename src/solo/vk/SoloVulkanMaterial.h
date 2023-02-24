/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloMaterial.h"
#include "SoloVulkan.h"
#include "SoloVulkanBuffer.h"
#include "SoloVulkanEffect.h"

namespace solo
{
    class Device;
    class Effect;
    class Camera;
    class VulkanTexture;
    class VulkanPipelineConfig;

    class VulkanMaterial final: public Material
    {
    public:
        struct UniformBufferItem
        {
            std::function<void(VulkanBuffer &, const Camera *, const Transform *)> write;
        };

        struct Sampler
        {
            u32 binding = 0;
            sptr<VulkanTexture> texture;
        };

        explicit VulkanMaterial(const sptr<Effect> &effect);
        VulkanMaterial(const VulkanMaterial &other) = default;
        ~VulkanMaterial() = default;

        auto effect() const -> sptr<Effect> override
        {
            return effect_;
        }

        auto clone() const -> sptr<Material> override
        {
            return std::make_shared<VulkanMaterial>(*this);
        }

        void setFloatParameter(const str &name, float value) override;
        void setVector2Parameter(const str &name, const Vector2 &value) override;
        void setVector3Parameter(const str &name, const Vector3 &value) override;
        void setVector4Parameter(const str &name, const Vector4 &value) override;
        void setMatrixParameter(const str &name, const Matrix &value) override;
        void setTextureParameter(const str &name, sptr<Texture> value) override;

        void bindFloatParameter(const str &name, const std::function<float()> &valueGetter) override;
        void bindVector2Parameter(const str &name, const std::function<Vector2()> &valueGetter) override;
        void bindVector3Parameter(const str &name, const std::function<Vector3()> &valueGetter) override;
        void bindVector4Parameter(const str &name, const std::function<Vector4()> &valueGetter) override;
        void bindMatrixParameter(const str &name, const std::function<Matrix()> &valueGetter) override;

        void bindParameter(const str &name, ParameterBinding binding) override;

        auto samplers() const -> umap<str, Sampler> const &
        {
            return samplers_;
        }
        auto bufferItems() const -> umap<str, umap<str, UniformBufferItem>> const &
        {
            return bufferItems_;    // TODO rename
        }
        auto stateHash() const -> size_t;

    private:
        using ParameterWriteFunc = std::function<void(VulkanBuffer &, u32, u32, const Camera *, const Transform *)>;

        sptr<VulkanEffect> effect_;
        umap<str, umap<str, UniformBufferItem>> bufferItems_;
        umap<str, Sampler> samplers_;

        void setParameter(const str &name, const ParameterWriteFunc &write);
    };
}

#endif
