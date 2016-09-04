#include "SoloVulkanPipeline.h"

using namespace solo;


auto createShaderStageInfo(bool vertex, VkShaderModule shader, const char* entryPoint) -> VkPipelineShaderStageCreateInfo
{
    VkPipelineShaderStageCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.stage = vertex ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;
    info.module = shader;
    info.pName = entryPoint;
    info.pSpecializationInfo = nullptr;
    return info;
}


VulkanPipeline::VulkanPipeline(VkDevice device):
    device(device)
{
}


VulkanPipeline::~VulkanPipeline()
{
    if (pipeline)
        vkDestroyPipeline(device, pipeline, nullptr);
}


void VulkanPipeline::setVertexShader(VkShaderModule shader, const char* entryPoint)
{
    vertexShader = shader;
    vertexShaderStageInfo = createShaderStageInfo(true, shader, entryPoint);
}


void VulkanPipeline::setFragmentShader(VkShaderModule shader, const char* entryPoint)
{
    fragmentShader = shader;
    fragmentShaderStageInfo = createShaderStageInfo(true, shader, entryPoint);
}


void VulkanPipeline::rebuild()
{
    // TODO descriptor sets and write sets
    VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = 0;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    VkPipelineLayout layout;
    SL_CHECK_VK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &layout));

    // ...
}
