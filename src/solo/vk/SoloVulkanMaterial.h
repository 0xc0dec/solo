/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloMaterial.h"
#include "SoloVulkan.h"
#include "SoloVulkanDescriptorPool.h"
#include "SoloVulkanBuffer.h"
#include "SoloVulkanEffect.h"
#include "SoloMap.h"

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

            void applyParameters(Renderer *renderer, const Camera *camera, const Transform *nodeTransform);

            auto getDescSetLayout(const Camera *camera, const Transform *nodeTransform) const -> VkDescriptorSetLayout; // TODO remove/make better
            auto getDescSet(const Camera *camera, const Transform *nodeTransform) const -> VkDescriptorSet; // TODO remove/make better
            auto getCullModeFlags() const -> VkCullModeFlags;
            auto getVkPolygonMode() const -> VkPolygonMode;

        private:
            struct UniformBufferItem
            {
                std::function<void(Buffer&, const Camera*, const Transform*)> write;
            };

            struct SamplerInfo
            {
                u32 binding;
                sptr<Texture> texture;
            };

            struct NodeBinding
            {
                umap<str, Buffer> buffers;
                DescriptorPool descPool;
                Resource<VkDescriptorSetLayout> descSetLayout;
                VkDescriptorSet descSet = VK_NULL_HANDLE;
            };

            using ParameterWriteFunc = std::function<void(Buffer&, u32, u32, const Camera*, const Transform*)>;

            sptr<Effect> effect;

            umap<const Transform*, umap<const Camera*, NodeBinding>> nodeBindings;
            umap<str, umap<str, UniformBufferItem>> bufferItems;
            umap<str, SamplerInfo> samplers;

            void setUniformParameter(const str &name, ParameterWriteFunc write);
        };
    }
}

#endif