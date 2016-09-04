#pragma once

#include "SoloCommon.h"
#include "SoloVulkan.h"

namespace solo
{
    class VulkanPipeline
    {
    public:
        VulkanPipeline(VkDevice device);
        ~VulkanPipeline();
        SL_NONCOPYABLE(VulkanPipeline)

        void setVertexShader(VkShaderModule shader, const char* entryPoint);
        void setFragmentShader(VkShaderModule shader, const char* entryPoint);

        void setVertexAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset);

        void setTopology(VkPrimitiveTopology topology);

        void setCullMode(VkCullModeFlags mode);
        void setFrontFace(VkFrontFace frontFace);

        void setRasterizationSampleCount(VkSampleCountFlagBits sampleCount);

        void rebuild();

    private:
        VkDevice device = nullptr;
        VkPipeline pipeline = nullptr;

        VkShaderModule vertexShader = nullptr;
        VkShaderModule fragmentShader = nullptr;
        VkPipelineShaderStageCreateInfo vertexShaderStageInfo;
        VkPipelineShaderStageCreateInfo fragmentShaderStageInfo;

        std::vector<VkVertexInputAttributeDescription> vertexAttrs;

        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        VkPipelineRasterizationStateCreateInfo rasterState;
        VkPipelineMultisampleStateCreateInfo multisampleState;
    };

    inline void VulkanPipeline::setTopology(VkPrimitiveTopology topology)
    {
        this->topology = topology;
    }

    inline void VulkanPipeline::setCullMode(VkCullModeFlags mode)
    {
        rasterState.cullMode = mode;
    }

    inline void VulkanPipeline::setFrontFace(VkFrontFace frontFace)
    {
        rasterState.frontFace = frontFace;
    }

    inline void VulkanPipeline::setRasterizationSampleCount(VkSampleCountFlagBits sampleCount)
    {
        multisampleState.rasterizationSamples = sampleCount;
    }
}