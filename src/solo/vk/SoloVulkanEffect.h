/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloEffect.h"
#include "SoloVulkan.h"
#include "SoloMap.h"

namespace solo
{
    namespace vk
    {
        class Renderer;

        class Effect final: public solo::Effect
        {
        public:
            struct UniformBufferMemberInfo
            {
                u32 offset;
                u32 size;
            };

            struct UniformBufferInfo
            {
                umap<str, UniformBufferMemberInfo> members;
                u32 binding;
                u32 size;
            };

            struct SamplerInfo
            {
                u32 binding;
            };

            static auto createFromPrefab(Device *device, EffectPrefab prefab) -> sptr<Effect>;

            Effect(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen);
            ~Effect();

            auto getVertexShader() const -> VkShaderModule { return vertexShader; }
            auto getFragmentShader() const -> VkShaderModule { return fragmentShader; }

            auto getUniformBufferInfo(const str &bufferName) -> UniformBufferInfo;
            auto getSamplerInfo(const str &samplerName) -> SamplerInfo;

            auto getUniformBuffers() const -> umap<str, UniformBufferInfo> const& { return uniformBuffers; }

        private:
            Renderer *renderer = nullptr;
            Resource<VkShaderModule> vertexShader;
            Resource<VkShaderModule> fragmentShader;
            
            umap<str, UniformBufferInfo> uniformBuffers;
            umap<str, SamplerInfo> samplers;

            void introspectShader(const u32 *src, u32 len);
        };
    }
}

#endif
