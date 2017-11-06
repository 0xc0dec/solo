/*
    Copyright (c) Aleksey Fedotov
    MIT license
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

    namespace vk
    {
        class Renderer;
        class Texture;

        class Material final: public solo::Material
        {
        public:
            struct UniformBufferItem
            {
                std::function<void(Buffer&, const Camera*, const Transform*)> write;
            };

            struct Sampler
            {
                u32 binding;
                sptr<Texture> texture;
            };

            Material(sptr<solo::Effect> effect);
            ~Material();

            auto getEffect() const -> solo::Effect* override final { return effect.get(); }

            void setFloatParameter(const str &name, float value) override final;
            void setVector2Parameter(const str &name, const Vector2 &value) override final;
            void setVector3Parameter(const str &name, const Vector3 &value) override final;
            void setVector4Parameter(const str &name, const Vector4 &value) override final;
            void setMatrixParameter(const str &name, const Matrix &value) override final;
            void setTextureParameter(const str &name, sptr<solo::Texture> value) override final;
            
            void bindParameter(const str &name, BindParameterSemantics semantics) override final;

            auto getSamplers() const -> umap<str, Sampler> const& { return samplers; }
            auto getBufferItems() const -> umap<str, umap<str, UniformBufferItem>> const& { return bufferItems; } // TODO rename

            auto getCullModeFlags() const -> VkCullModeFlags;
            auto getVkPolygonMode() const -> VkPolygonMode;

        private:
            using ParameterWriteFunc = std::function<void(Buffer&, u32, u32, const Camera*, const Transform*)>;

            sptr<Effect> effect;

            umap<str, umap<str, UniformBufferItem>> bufferItems;
            umap<str, Sampler> samplers;

            void setUniformParameter(const str &name, ParameterWriteFunc write);
        };
    }
}

#endif