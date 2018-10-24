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
    class VulkanRenderer;

    class VulkanEffect final: public Effect
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
            u32 binding = 0;
            u32 size = 0;
        };

        struct Sampler
        {
            u32 binding;
        };

        struct VertexAttribute
        {
            u32 location;
        };

        static auto createFromSources(Device *device,
            const void *vsSrc, u32 vsSrcLen, const str &vsFileName,
            const void *fsSrc, u32 fsSrcLen, const str &fsFileName) -> sptr<VulkanEffect>;

        VulkanEffect(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen);
        ~VulkanEffect() = default;

        auto vertexShaderModule() const -> VkShaderModule { return vertexShader_; }
        auto fragmentShaderModule() const -> VkShaderModule { return fragmentShader_; }

        auto uniformBuffer(const str &name) -> UniformBuffer;
        auto sampler(const str &name) -> Sampler;

        auto uniformBuffers() const -> umap<str, UniformBuffer> const& { return uniformBuffers_; }
        auto samplers() const -> umap<str, Sampler> const& { return samplers_; }
        auto vertexAttributes() const -> umap<str, VertexAttribute> const& { return vertexAttributes_; }

    private:
        VulkanRenderer *renderer_ = nullptr;
        VulkanResource<VkShaderModule> vertexShader_;
        VulkanResource<VkShaderModule> fragmentShader_;
            
        umap<str, UniformBuffer> uniformBuffers_;
        umap<str, Sampler> samplers_;
        umap<str, VertexAttribute> vertexAttributes_;

        void introspectShader(const u32 *src, u32 len, bool vertex);
    };
}

#endif
