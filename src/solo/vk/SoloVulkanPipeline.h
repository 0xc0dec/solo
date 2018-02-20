/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"

namespace solo
{
    class VulkanPipelineConfig
    {
    public:
        VulkanPipelineConfig(VkShaderModule vertexShader, VkShaderModule fragmentShader);
        ~VulkanPipelineConfig() = default;

        auto withColorBlendAttachmentCount(u32 count) -> VulkanPipelineConfig&;
        auto withVertexAttribute(u32 location, u32 binding, VkFormat format, u32 offset) -> VulkanPipelineConfig&;
        auto withVertexBinding(u32 binding, u32 stride, VkVertexInputRate inputRate) -> VulkanPipelineConfig&;
        auto withDescriptorSetLayout(VkDescriptorSetLayout layout) -> VulkanPipelineConfig&;
        auto withFrontFace(VkFrontFace frontFace) -> VulkanPipelineConfig&;
        auto withCullMode(VkCullModeFlags cullFlags) -> VulkanPipelineConfig&;
        auto withDepthTest(bool write, bool test) -> VulkanPipelineConfig&;
        auto withBlend(bool enabled, VkBlendFactor srcColorFactor, VkBlendFactor dstColorFactor,
            VkBlendFactor srcAlphaFactor, VkBlendFactor dstAlphaFactor) -> VulkanPipelineConfig&;
        auto withTopology(VkPrimitiveTopology topology) -> VulkanPipelineConfig&;
        auto withPolygonMode(VkPolygonMode mode) -> VulkanPipelineConfig&;

    private:
        friend class VulkanPipeline;

        VkShaderModule vertexShader;
        VkShaderModule fragmentShader;
        VkPipelineRasterizationStateCreateInfo rasterStateInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
        u32 colorBlendAttachmentCount = 0;

        vec<VkVertexInputAttributeDescription> vertexAttrs;
        vec<VkVertexInputBindingDescription> vertexBindings;
        vec<VkDescriptorSetLayout> descSetLayouts;

        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    };

    class VulkanPipeline
    {
    public:
        VulkanPipeline() = default;
        VulkanPipeline(VkDevice device, VkRenderPass renderPass, const VulkanPipelineConfig &config);
        VulkanPipeline(const VulkanPipeline &other) = delete;
        VulkanPipeline(VulkanPipeline &&other) = default;
        ~VulkanPipeline() = default;

        auto operator=(const VulkanPipeline &other) -> VulkanPipeline& = delete;
        auto operator=(VulkanPipeline &&other) -> VulkanPipeline& = default;

        operator VkPipeline() { return pipeline; }

        auto getHandle() const -> VkPipeline { return pipeline; }
        auto getLayout() const -> VkPipelineLayout { return layout; }

    private:
        VulkanResource<VkPipeline> pipeline;
        VulkanResource<VkPipelineLayout> layout;
    };

    inline auto VulkanPipelineConfig::withTopology(VkPrimitiveTopology topology) -> VulkanPipelineConfig&
    {
        this->topology = topology;
        return *this;
    }

    inline auto VulkanPipelineConfig::withPolygonMode(VkPolygonMode mode) -> VulkanPipelineConfig&
    {
        rasterStateInfo.polygonMode = mode;
        return *this;
    }

    inline auto VulkanPipelineConfig::withDescriptorSetLayout(VkDescriptorSetLayout layout) -> VulkanPipelineConfig&
    {
        descSetLayouts.push_back(layout);
        return *this;
    }

    inline auto VulkanPipelineConfig::withFrontFace(VkFrontFace frontFace) -> VulkanPipelineConfig&
    {
        rasterStateInfo.frontFace = frontFace;
        return *this;
    }

    inline auto VulkanPipelineConfig::withCullMode(VkCullModeFlags cullFlags) -> VulkanPipelineConfig&
    {
        rasterStateInfo.cullMode = cullFlags;
        return *this;
    }
}

#endif
