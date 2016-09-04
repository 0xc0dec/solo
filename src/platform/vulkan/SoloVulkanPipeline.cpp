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


auto createRasterizationStateInfo(bool depthClamp, bool discardEnabled, VkCullModeFlags cullMode, VkFrontFace frontFace)
    -> VkPipelineRasterizationStateCreateInfo
{
    VkPipelineRasterizationStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.depthClampEnable = depthClamp;
    info.rasterizerDiscardEnable = discardEnabled;
    info.polygonMode = VK_POLYGON_MODE_FILL;
    info.cullMode = cullMode;
    info.frontFace = frontFace;
    info.depthBiasEnable = false;
    info.depthBiasClamp = 0;
    info.depthBiasConstantFactor = 0;
    info.depthBiasClamp = 0;
    info.depthBiasSlopeFactor = 0;
    info.lineWidth = 0;
    return info;
}


auto createMultisampleStateInfo(VkSampleCountFlagBits rasterizationSampleCount) -> VkPipelineMultisampleStateCreateInfo
{
    VkPipelineMultisampleStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.rasterizationSamples = rasterizationSampleCount;
    info.sampleShadingEnable = false;
    info.minSampleShading = 0;
    info.pSampleMask = nullptr;
    info.alphaToCoverageEnable = false;
    info.alphaToOneEnable = false;
    return info;
}


VulkanPipeline::VulkanPipeline(VkDevice device):
    device(device)
{
    rasterState = createRasterizationStateInfo(false, false, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
    multisampleState = createMultisampleStateInfo(VK_SAMPLE_COUNT_1_BIT);
}


VulkanPipeline::~VulkanPipeline()
{
    if (pipeline)
        vkDestroyPipeline(device, pipeline, nullptr);
}


void VulkanPipeline::setVertexShader(VkShaderModule shader, const char* entryPoint)
{
    vertexShader = shader;
    if (shader)
        vertexShaderStageInfo = createShaderStageInfo(true, shader, entryPoint);
}


void VulkanPipeline::setFragmentShader(VkShaderModule shader, const char* entryPoint)
{
    fragmentShader = shader;
    if (shader)
        fragmentShaderStageInfo = createShaderStageInfo(false, shader, entryPoint);
}


void VulkanPipeline::setVertexAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset)
{
    if (vertexAttrs.size() <= location)
        vertexAttrs.resize(location + 1);
    vertexAttrs[location].location = location;
    vertexAttrs[location].binding = binding;
    vertexAttrs[location].format = format;
    vertexAttrs[location].offset = offset;
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

    std::vector<VkPipelineShaderStageCreateInfo> shaderStageStates;
    if (vertexShader)
        shaderStageStates.push_back(vertexShaderStageInfo);
    if (fragmentShader)
        shaderStageStates.push_back(fragmentShaderStageInfo);

    const uint32_t TODO_vertexSize = 123;

    VkVertexInputBindingDescription vertexInputBindingDesc {};
    vertexInputBindingDesc.binding = 0;
    vertexInputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vertexInputBindingDesc.stride = TODO_vertexSize;
    std::vector<VkVertexInputBindingDescription> inputBindingDescriptions = { vertexInputBindingDesc };

    VkPipelineVertexInputStateCreateInfo vertexInputState {};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.pNext = nullptr;
    vertexInputState.flags = 0;
    vertexInputState.vertexBindingDescriptionCount = static_cast<uint32_t>(inputBindingDescriptions.size());
    vertexInputState.pVertexBindingDescriptions = inputBindingDescriptions.data();
    vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttrs.size());
    vertexInputState.pVertexAttributeDescriptions = vertexAttrs.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState {};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.pNext = nullptr;
    inputAssemblyState.flags = 0;
    inputAssemblyState.topology = topology;
    inputAssemblyState.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;
    viewportState.flags = 0;
    viewportState.viewportCount = 1;
    viewportState.pViewports = nullptr;
    viewportState.scissorCount = 1;
    viewportState.pScissors = nullptr;

    // TODO continue
}
