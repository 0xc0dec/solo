#include "SoloVulkanPipeline.h"

using namespace solo;


VulkanPipeline::VulkanPipeline(VkDevice device, VkRenderPass renderPass):
    device(device),
    renderPass(renderPass)
{
    rasterState = VulkanHelper::createRasterizationStateInfo(false, false, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
    multisampleState = VulkanHelper::createMultisampleStateInfo(VK_SAMPLE_COUNT_1_BIT);
    blendAttachmentState = VulkanHelper::createBlendAttachmentState(false,
        VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
        VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
    colorBlendState = VulkanHelper::createColorBlendStateInfo(&blendAttachmentState, false, VK_LOGIC_OP_COPY);
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
        vertexShaderStageInfo = VulkanHelper::createShaderStageInfo(true, shader, entryPoint);
}


void VulkanPipeline::setFragmentShader(VkShaderModule shader, const char* entryPoint)
{
    fragmentShader = shader;
    if (shader)
        fragmentShaderStageInfo = VulkanHelper::createShaderStageInfo(false, shader, entryPoint);
}


void VulkanPipeline::setVertexAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset)
{
    if (location >= vertexAttrs.size())
        vertexAttrs.resize(location + 1);
    vertexAttrs[location].location = location;
    vertexAttrs[location].binding = binding;
    vertexAttrs[location].format = format;
    vertexAttrs[location].offset = offset;
}


void VulkanPipeline::resetVertexAttributes()
{
    vertexAttrs.clear();
}


void VulkanPipeline::setDescriptorSetLayouts(VkDescriptorSetLayout* layouts, uint32_t count)
{
    descSetLayouts.resize(count);
    for (auto i = 0; i < count; i++)
        descSetLayouts[i] = layouts[i];
}


void VulkanPipeline::rebuild()
{
    // TODO descriptor sets and write sets
    VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = 0;
    pipelineLayoutInfo.setLayoutCount = descSetLayouts.size();
    pipelineLayoutInfo.pSetLayouts = descSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    VkPipelineLayout layout;
    SL_CHECK_VK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &layout));

    std::vector<VkPipelineShaderStageCreateInfo> shaderStageStates;
    if (vertexShader)
        shaderStageStates.push_back(vertexShaderStageInfo);
    if (fragmentShader)
        shaderStageStates.push_back(fragmentShaderStageInfo);

    VkVertexInputBindingDescription vertexInputBindingDesc {}; // TODO other bindings
    vertexInputBindingDesc.binding = 0;
    vertexInputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vertexInputBindingDesc.stride = vertexSize;

    VkPipelineVertexInputStateCreateInfo vertexInputState {};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.pNext = nullptr;
    vertexInputState.flags = 0;
    vertexInputState.vertexBindingDescriptionCount = 1;
    vertexInputState.pVertexBindingDescriptions = &vertexInputBindingDesc;
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

    std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = nullptr;
    dynamicState.flags = 0;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkGraphicsPipelineCreateInfo pipelineInfo {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.flags = 0;
    pipelineInfo.stageCount = shaderStageStates.size();
    pipelineInfo.pStages = shaderStageStates.data();
    pipelineInfo.pVertexInputState = &vertexInputState;
    pipelineInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineInfo.pTessellationState = nullptr;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterState;
    pipelineInfo.pMultisampleState = &multisampleState;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlendState;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = layout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;
    pipelineInfo.basePipelineIndex = -1;

    SL_CHECK_VK_RESULT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));
}


void VulkanPipeline::bind(VkCommandBuffer cmdBuf)
{
    vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}
