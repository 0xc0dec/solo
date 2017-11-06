/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloEffect.h"
#include "SoloVulkan.h"

namespace solo
{
    namespace vk
    {
        class Renderer;

        class Effect final: public solo::Effect
        {
        public:
            struct UniformBufferMember
            {
                u32 offset;
                u32 size;
            };

            struct UniformBuffer
            {
                umap<str, UniformBufferMember> members;
                u32 binding;
                u32 size;
            };

            struct Sampler
            {
                u32 binding;
            };

            static auto createFromPrefab(Device *device, EffectPrefab prefab) -> sptr<Effect>;

            Effect(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen);
            ~Effect();

            auto getVertexShader() const -> VkShaderModule { return vertexShader; }
            auto getFragmentShader() const -> VkShaderModule { return fragmentShader; }

            auto getUniformBuffer(const str &name) -> UniformBuffer;
            auto getSampler(const str &name) -> Sampler;

            auto getUniformBuffers() const -> umap<str, UniformBuffer> const& { return uniformBuffers; }
            auto getSamplers() const -> umap<str, Sampler> const& { return samplers; }

        private:
            Renderer *renderer = nullptr;
            Resource<VkShaderModule> vertexShader;
            Resource<VkShaderModule> fragmentShader;
            
            umap<str, UniformBuffer> uniformBuffers;
            umap<str, Sampler> samplers;

            void introspectShader(const u32 *src, u32 len);
        };
    }
}

#endif
