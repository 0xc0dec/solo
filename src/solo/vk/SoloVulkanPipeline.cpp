/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanPipeline.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloMaterial.h"

using namespace solo;

static auto createShaderStageInfo(bool vertex, VkShaderModule shader, const s8* entryPoint) -> VkPipelineShaderStageCreateInfo
{
    VkPipelineShaderStageCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.stage = vertex ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;
    info.module = shader;
    info.pName = entryPoint;
    info.pSpecializationInfo = nullptr;
    return info;
}

VulkanPipeline::VulkanPipeline(VkDevice device, VkRenderPass renderPass, const VulkanPipelineConfig &config)
{
    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.pNext = nullptr;
    layoutInfo.flags = 0;
    layoutInfo.setLayoutCount = config.descSetLayouts.size();
    layoutInfo.pSetLayouts = config.descSetLayouts.data();
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = nullptr;

    VulkanResource<VkPipelineLayout> layout{device, vkDestroyPipelineLayout};
    SL_VK_CHECK_RESULT(vkCreatePipelineLayout(device, &layoutInfo, nullptr, layout.cleanRef()));

    VkPipelineMultisampleStateCreateInfo multisampleState{};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.pNext = nullptr;
    multisampleState.flags = 0;
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleState.sampleShadingEnable = false;
    multisampleState.minSampleShading = 0;
    multisampleState.pSampleMask = nullptr;
    multisampleState.alphaToCoverageEnable = false;
    multisampleState.alphaToOneEnable = false;

    VkPipelineColorBlendStateCreateInfo colorBlendState{};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.pNext = nullptr;
    colorBlendState.flags = 0;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.logicOp = VK_LOGIC_OP_COPY;
    colorBlendState.attachmentCount = config.colorBlendAttachmentCount;
    colorBlendState.pAttachments = &config.colorBlendAttachmentState;
    colorBlendState.blendConstants[0] = 0;
    colorBlendState.blendConstants[1] = 0;
    colorBlendState.blendConstants[2] = 0;
    colorBlendState.blendConstants[3] = 0;

    const auto vertexShaderStageInfo = createShaderStageInfo(true, config.vertexShader, "main");
    const auto fragmentShaderStageInfo = createShaderStageInfo(false, config.fragmentShader, "main");

    vec<VkPipelineShaderStageCreateInfo> shaderStageStates{vertexShaderStageInfo, fragmentShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputState{};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.pNext = nullptr;
    vertexInputState.flags = 0;
    vertexInputState.vertexBindingDescriptionCount = config.vertexBindings.size();
    vertexInputState.pVertexBindingDescriptions = config.vertexBindings.data();
    vertexInputState.vertexAttributeDescriptionCount = config.vertexAttrs.size();
    vertexInputState.pVertexAttributeDescriptions = config.vertexAttrs.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.pNext = nullptr;
    inputAssemblyState.flags = 0;
    inputAssemblyState.topology = config.topology;
    inputAssemblyState.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;
    viewportState.flags = 0;
    viewportState.viewportCount = 1;
    viewportState.pViewports = nullptr;
    viewportState.scissorCount = 1;
    viewportState.pScissors = nullptr;

    vec<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = nullptr;
    dynamicState.flags = 0;
    dynamicState.dynamicStateCount = dynamicStates.size();
    dynamicState.pDynamicStates = dynamicStates.data();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.flags = 0;
    pipelineInfo.stageCount = shaderStageStates.size();
    pipelineInfo.pStages = shaderStageStates.data();
    pipelineInfo.pVertexInputState = &vertexInputState;
    pipelineInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineInfo.pTessellationState = nullptr;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &config.rasterStateInfo;
    pipelineInfo.pMultisampleState = &multisampleState;
    pipelineInfo.pDepthStencilState = &config.depthStencilStateInfo;
    pipelineInfo.pColorBlendState = &colorBlendState;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = layout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    VulkanResource<VkPipeline> pipeline{device, vkDestroyPipeline};
    SL_VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, pipeline.cleanRef()));

    this->pipeline = std::move(pipeline);
    this->layout = std::move(layout);
}

VulkanPipelineConfig::VulkanPipelineConfig(VkShaderModule vertexShader, VkShaderModule fragmentShader):
    vertexShader(vertexShader),
    fragmentShader(fragmentShader),
    rasterStateInfo{},
    depthStencilStateInfo{},
    colorBlendAttachmentState{}
{
    rasterStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterStateInfo.pNext = nullptr;
    rasterStateInfo.flags = 0;
    rasterStateInfo.depthClampEnable = false;
    rasterStateInfo.rasterizerDiscardEnable = false;
    rasterStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterStateInfo.depthBiasEnable = false;
    rasterStateInfo.depthBiasClamp = 0;
    rasterStateInfo.depthBiasConstantFactor = 0;
    rasterStateInfo.depthBiasSlopeFactor = 0;
    rasterStateInfo.lineWidth = 1;

    depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilStateInfo.flags = 0;
    depthStencilStateInfo.depthTestEnable = true;
    depthStencilStateInfo.depthWriteEnable = true;
    depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencilStateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
    depthStencilStateInfo.front = depthStencilStateInfo.back;

    colorBlendAttachmentState.blendEnable = VK_FALSE;
    colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
}

auto VulkanPipelineConfig::withColorBlendAttachmentCount(u32 count) -> VulkanPipelineConfig &
{
    colorBlendAttachmentCount = count;
    return *this;
}

auto VulkanPipelineConfig::withVertexAttribute(u32 location, u32 binding, VkFormat format, u32 offset) -> VulkanPipelineConfig&
{
    VkVertexInputAttributeDescription desc{};
    desc.location = location;
    desc.binding = binding;
    desc.format = format;
    desc.offset = offset;
    vertexAttrs.push_back(desc);
    return *this;
}

auto VulkanPipelineConfig::withVertexBinding(u32 binding, u32 stride, VkVertexInputRate inputRate) -> VulkanPipelineConfig&
{
    VkVertexInputBindingDescription desc{};
    desc.binding = binding;
    desc.stride = stride;
    desc.inputRate = inputRate;
    vertexBindings.push_back(desc);
    return *this;
}

auto VulkanPipelineConfig::withDepthTest(bool write, bool test) -> VulkanPipelineConfig&
{
    depthStencilStateInfo.depthWriteEnable = write;
    depthStencilStateInfo.depthTestEnable = test;
    return *this;
}

auto VulkanPipelineConfig::withBlend(bool enabled,
    VkBlendFactor srcColorFactor, VkBlendFactor dstColorFactor,
    VkBlendFactor srcAlphaFactor, VkBlendFactor dstAlphaFactor) -> VulkanPipelineConfig&
{
    colorBlendAttachmentState.blendEnable = enabled ? VK_TRUE : VK_FALSE;
    colorBlendAttachmentState.srcColorBlendFactor = srcColorFactor;
    colorBlendAttachmentState.dstColorBlendFactor = dstColorFactor;
    colorBlendAttachmentState.srcAlphaBlendFactor = srcAlphaFactor;
    colorBlendAttachmentState.dstAlphaBlendFactor = dstAlphaFactor;
    return *this;
}

#endif
