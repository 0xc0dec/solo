#include "SoloVulkanPipeline.h"

using namespace solo;


VulkanPipeline::VulkanPipeline(VkDevice device, VkRenderPass renderPass):
    device(device),
    renderPass(renderPass)
{
    rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterState.pNext = nullptr;
    rasterState.flags = 0;
    rasterState.depthClampEnable = false;
    rasterState.rasterizerDiscardEnable = false;
    rasterState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterState.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterState.depthBiasEnable = false;
    rasterState.depthBiasClamp = 0;
    rasterState.depthBiasConstantFactor = 0;
    rasterState.depthBiasClamp = 0;
    rasterState.depthBiasSlopeFactor = 0;
    rasterState.lineWidth = 1;

    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.pNext = nullptr;
    multisampleState.flags = 0;
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleState.sampleShadingEnable = false;
    multisampleState.minSampleShading = 0;
    multisampleState.pSampleMask = nullptr;
    multisampleState.alphaToCoverageEnable = false;
    multisampleState.alphaToOneEnable = false;

    blendAttachmentState.blendEnable = VK_FALSE;
    blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
    blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.pNext = nullptr;
    colorBlendState.flags = 0;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.logicOp = VK_LOGIC_OP_COPY;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &blendAttachmentState;
    colorBlendState.blendConstants[0] = 0;
    colorBlendState.blendConstants[1] = 0;
    colorBlendState.blendConstants[2] = 0;
    colorBlendState.blendConstants[3] = 0;

    vertexShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStage.pNext = nullptr;
    vertexShaderStage.flags = 0;
    vertexShaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStage.module = nullptr;
    vertexShaderStage.pName = nullptr;
    vertexShaderStage.pSpecializationInfo = nullptr;

    fragmentShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStage.pNext = nullptr;
    fragmentShaderStage.flags = 0;
    fragmentShaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStage.module = nullptr;
    fragmentShaderStage.pName = nullptr;
    fragmentShaderStage.pSpecializationInfo = nullptr;
}


VulkanPipeline::~VulkanPipeline()
{
    if (pipeline)
        vkDestroyPipeline(device, pipeline, nullptr);
}


void VulkanPipeline::setVertexShader(VkShaderModule shader, const char *entryPoint)
{
    vertexShaderStage.module = shader;
    vertexShaderStage.pName = entryPoint;
}


void VulkanPipeline::setFragmentShader(VkShaderModule shader, const char *entryPoint)
{
    fragmentShaderStage.module = shader;
    fragmentShaderStage.pName = entryPoint;
}


void VulkanPipeline::bind(VkCommandBuffer cmdBuf)
{
    vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}


void VulkanPipeline::cleanup()
{
    if (pipeline)
        vkDestroyPipeline(device, pipeline, nullptr);
    if (layout)
        vkDestroyPipelineLayout(device, layout, nullptr);
}


void VulkanPipeline::rebuild()
{
    cleanup();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = 0;
    pipelineLayoutInfo.setLayoutCount = 0; // descSetLayouts.size(); TODO
    pipelineLayoutInfo.pSetLayouts = nullptr; // descSetLayouts.data(); TODO
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    SL_CHECK_VK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &layout));

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

    std::vector<VkPipelineShaderStageCreateInfo> shaderStageStates;
    if (vertexShaderStage.module)
        shaderStageStates.push_back(vertexShaderStage);
    if (fragmentShaderStage.module)
        shaderStageStates.push_back(fragmentShaderStage);

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
