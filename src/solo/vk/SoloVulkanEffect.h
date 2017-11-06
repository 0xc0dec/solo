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
                uint32_t offset;
                uint32_t size;
            };

            struct UniformBufferInfo
            {
                umap<std::string, UniformBufferMemberInfo> members;
                uint32_t binding;
                uint32_t size;
            };

            struct SamplerInfo
            {
                uint32_t binding;
            };

            static auto createFromPrefab(Device *device, EffectPrefab prefab) -> sptr<Effect>;

            Effect(Device *device, const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen);
            ~Effect();

            auto getVertexShader() const -> VkShaderModule { return vertexShader; }
            auto getFragmentShader() const -> VkShaderModule { return fragmentShader; }

            auto getUniformBufferInfo(const std::string &bufferName) -> UniformBufferInfo;
            auto getSamplerInfo(const std::string &samplerName) -> SamplerInfo;

            auto getUniformBuffers() const -> umap<std::string, UniformBufferInfo> const& { return uniformBuffers; }

        private:
            Renderer *renderer = nullptr;
            Resource<VkShaderModule> vertexShader;
            Resource<VkShaderModule> fragmentShader;
            
            umap<std::string, UniformBufferInfo> uniformBuffers;
            umap<std::string, SamplerInfo> samplers;

            void introspectShader(const uint32_t *src, uint32_t len);
        };
    }
}

#endif
