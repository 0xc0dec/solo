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
#include <unordered_map>

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

            void setFloatParameter(const std::string &name, float value) override final;
            void setVector2Parameter(const std::string &name, const Vector2 &value) override final;
            void setVector3Parameter(const std::string &name, const Vector3 &value) override final;
            void setVector4Parameter(const std::string &name, const Vector4 &value) override final;
            void setMatrixParameter(const std::string &name, const Matrix &value) override final;
            void setTextureParameter(const std::string &name, sptr<solo::Texture> value) override final;
            
            void bindParameter(const std::string &name, BindParameterSemantics semantics) override final;

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
                uint32_t binding;
                sptr<Texture> texture;
            };

            struct NodeBinding
            {
                std::unordered_map<std::string, Buffer> buffers;
                DescriptorPool descPool;
                Resource<VkDescriptorSetLayout> descSetLayout;
                VkDescriptorSet descSet = VK_NULL_HANDLE;
            };

            using ParameterWriteFunc = std::function<void(Buffer&, uint32_t, uint32_t, const Camera*, const Transform*)>;

            sptr<Effect> effect;

            std::unordered_map<const Transform*, std::unordered_map<const Camera*, NodeBinding>> nodeBindings;
            std::unordered_map<std::string, std::unordered_map<std::string, UniformBufferItem>> bufferItems;
            std::unordered_map<std::string, SamplerInfo> samplers;

            void setUniformParameter(const std::string &name, ParameterWriteFunc write);
        };
    }
}

#endif