#pragma once

#include "SoloCommon.h"
#include "SoloVulkan.h"
#include <vector>

namespace solo
{
    class VulkanPipeline
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(VulkanPipeline)

        VulkanPipeline(VkDevice device, VkRenderPass renderPass);
        ~VulkanPipeline();

        void setVertexShader(VkShaderModule shader, const char* entryPoint);
        void setFragmentShader(VkShaderModule shader, const char* entryPoint);

        void rebuild();
        void bind(VkCommandBuffer cmdBuf);

    private:
        void cleanup();

        VkDevice device = nullptr;
        VkRenderPass renderPass = nullptr;
        VkPipeline pipeline = nullptr;
        VkPipelineLayout layout = nullptr;
        VkPipelineShaderStageCreateInfo vertexShaderStage;
        VkPipelineShaderStageCreateInfo fragmentShaderStage;

        std::vector<VkVertexInputAttributeDescription> vertexAttrs;
        std::vector<VkDescriptorSetLayout> descSetLayouts;
        uint32_t vertexSize = 0;
        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        VkPipelineRasterizationStateCreateInfo rasterState;
        VkPipelineMultisampleStateCreateInfo multisampleState;
        VkPipelineColorBlendAttachmentState blendAttachmentState;
        VkPipelineColorBlendStateCreateInfo colorBlendState;
    };
}