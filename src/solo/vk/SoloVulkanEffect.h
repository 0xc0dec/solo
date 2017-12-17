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
            u32 binding;
            u32 size;
        };

        struct Sampler
        {
            u32 binding;
        };

        struct VertexAttribute
        {
            u32 location;
        };

        static auto createFromSource(Device *device,
            const void *vsSrc, u32 vsSrcLen, const str &vsFileName,
            const void *fsSrc, u32 fsSrcLen, const str &fsFileName) -> sptr<VulkanEffect>;

        VulkanEffect(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen);
        ~VulkanEffect();

        auto getVkVertexShader() const -> VkShaderModule { return vertexShader; }
        auto getVkFragmentShader() const -> VkShaderModule { return fragmentShader; }

        auto getUniformBuffer(const str &name) -> UniformBuffer;
        auto getSampler(const str &name) -> Sampler;

        auto getUniformBuffers() const -> umap<str, UniformBuffer> const& { return uniformBuffers; }
        auto getSamplers() const -> umap<str, Sampler> const& { return samplers; }
        auto getVertexAttributes() const -> umap<str, VertexAttribute> const& { return vertexAttributes; }

    private:
        VulkanRenderer *renderer = nullptr;
        VulkanResource<VkShaderModule> vertexShader;
        VulkanResource<VkShaderModule> fragmentShader;
            
        umap<str, UniformBuffer> uniformBuffers;
        umap<str, Sampler> samplers;
        umap<str, VertexAttribute> vertexAttributes;

        void introspectShader(const u32 *src, u32 len, bool vertex);
    };
}

#endif
