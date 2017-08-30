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
            static auto create(Device *device, EffectPrefab prefab) -> sptr<Effect>;

            Effect(Device *device, const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen);
            Effect(Device *device, const std::string &vsSrc, const std::string &fsSrc);
            ~Effect();

            auto getVertexShader() const -> VkShaderModule;
            auto getFragmentShader() const -> VkShaderModule;

        private:
            Renderer *renderer = nullptr;
            Resource<VkShaderModule> vertexShader;
            Resource<VkShaderModule> fragmentShader;
        };

        inline auto Effect::getVertexShader() const -> VkShaderModule
        {
            return vertexShader;
        }

        inline auto Effect::getFragmentShader() const -> VkShaderModule
        {
            return fragmentShader;
        }
    }
}

#endif
