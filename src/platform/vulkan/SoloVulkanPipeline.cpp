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


auto createBlendAttachmentState(bool blendEnabled, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor,
    VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp,
    VkColorComponentFlags colorWriteMask) -> VkPipelineColorBlendAttachmentState
{
    VkPipelineColorBlendAttachmentState state {};
    state.blendEnable = blendEnabled ? VK_TRUE : VK_FALSE;
    state.srcColorBlendFactor = srcColorBlendFactor;
    state.dstColorBlendFactor = dstColorBlendFactor;
    state.colorBlendOp = colorBlendOp;
    state.srcAlphaBlendFactor = srcAlphaBlendFactor;
    state.dstAlphaBlendFactor = dstAlphaBlendFactor;
    state.alphaBlendOp = alphaBlendOp;
    state.colorWriteMask = colorWriteMask;
    return state;
}


auto createColorBlendStateInfo(VkPipelineColorBlendAttachmentState* blendAttachments, bool logicOpEnabled, VkLogicOp logicOp)
    -> VkPipelineColorBlendStateCreateInfo
{
    VkPipelineColorBlendStateCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.logicOpEnable = logicOpEnabled ? VK_TRUE : VK_FALSE;
    info.logicOp = logicOp;
    info.attachmentCount = 1;
    info.pAttachments = blendAttachments;
    info.blendConstants[0] = 0;
    info.blendConstants[1] = 0;
    info.blendConstants[2] = 0;
    info.blendConstants[3] = 0;
    return info;
}


VulkanPipeline::VulkanPipeline(VkDevice device, VkRenderPass renderPass):
    device(device),
    renderPass(renderPass)
{
    rasterState = createRasterizationStateInfo(false, false, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
    multisampleState = createMultisampleStateInfo(VK_SAMPLE_COUNT_1_BIT);
    blendAttachmentState = createBlendAttachmentState(false,
        VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
        VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
    colorBlendState = createColorBlendStateInfo(&blendAttachmentState, false, VK_LOGIC_OP_COPY);
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


void VulkanPipeline::resetVertexAttributes()
{
    vertexAttrs.clear();
}


void VulkanPipeline::setDescriptorSet(/* TODO */)
{
    // TODO todo todo...
    VkDescriptorSetLayoutBinding binding {};
	binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	binding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
	binding.binding = 0;
	binding.descriptorCount = 1;

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo {};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutCreateInfo.pNext = nullptr;
	layoutCreateInfo.pBindings = &binding;
	layoutCreateInfo.bindingCount = 1;

    VkDescriptorSetLayout descSetLayout = nullptr;
    SL_CHECK_VK_RESULT(vkCreateDescriptorSetLayout(device, &layoutCreateInfo, nullptr, &descSetLayout));

    descSetLayouts.push_back(descSetLayout);
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
