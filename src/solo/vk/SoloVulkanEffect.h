/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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

        static auto fromSources(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen)
            -> sptr<VulkanEffect>;

        VulkanEffect(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen);
        ~VulkanEffect() = default;

        auto vsModule() const -> VkShaderModule { return vs_; }
        auto fsModule() const -> VkShaderModule { return fs_; }

    	auto hasUniformBuffer(const str &name) const -> bool { return uniformBuffers_.count(name); }
        auto uniformBuffer(const str &name) const -> UniformBuffer { return uniformBuffers_.at(name); }
        auto sampler(const str &name) -> Sampler;

        auto uniformBuffers() const -> umap<str, UniformBuffer> const& { return uniformBuffers_; }
        auto samplers() const -> umap<str, Sampler> const& { return samplers_; }
        auto vertexAttributes() const -> umap<str, VertexAttribute> const& { return vertexAttributes_; }

    private:
        VulkanRenderer *renderer_ = nullptr;
        VulkanResource<VkShaderModule> vs_;
        VulkanResource<VkShaderModule> fs_;
            
        umap<str, UniformBuffer> uniformBuffers_;
        umap<str, Sampler> samplers_;
        umap<str, VertexAttribute> vertexAttributes_;

        void introspectShader(const u32 *src, u32 len, bool vertex);
    };
}

#endif
