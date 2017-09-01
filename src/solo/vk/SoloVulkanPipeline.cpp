/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanPipeline.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;
using namespace vk;

Pipeline::Pipeline(VkDevice device, VkRenderPass renderPass, const PipelineConfig &config)
{
    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.pNext = nullptr;
    layoutInfo.flags = 0;
    layoutInfo.setLayoutCount = config.descSetLayouts.size();
    layoutInfo.pSetLayouts = config.descSetLayouts.data();
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = nullptr;

    Resource<VkPipelineLayout> layout{device, vkDestroyPipelineLayout};
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
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &config.blendAttachmentState;
    colorBlendState.blendConstants[0] = 0;
    colorBlendState.blendConstants[1] = 0;
    colorBlendState.blendConstants[2] = 0;
    colorBlendState.blendConstants[3] = 0;

    auto vertexShaderStageInfo = createShaderStageInfo(true, config.vertexShader, "main");
    auto fragmentShaderStageInfo = createShaderStageInfo(false, config.fragmentShader, "main");

    std::vector<VkPipelineShaderStageCreateInfo> shaderStageStates{vertexShaderStageInfo, fragmentShaderStageInfo};

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

    std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
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

    Resource<VkPipeline> pipeline{device, vkDestroyPipeline};
    SL_VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, pipeline.cleanRef()));

    this->pipeline = std::move(pipeline);
    this->layout = std::move(layout);
}

PipelineConfig::PipelineConfig(VkShaderModule vertexShader, VkShaderModule fragmentShader):
    vertexShader(vertexShader),
    fragmentShader(fragmentShader),
    rasterStateInfo{},
    depthStencilStateInfo{}
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

    blendAttachmentState.blendEnable = VK_FALSE;
    blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
    blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
}

auto PipelineConfig::withVertexAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset) -> PipelineConfig&
{
    if (location >= vertexAttrs.size())
        vertexAttrs.resize(location + 1);
    vertexAttrs[location].location = location;
    vertexAttrs[location].binding = binding;
    vertexAttrs[location].format = format;
    vertexAttrs[location].offset = offset;
    return *this;
}

auto PipelineConfig::withVertexBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate) -> PipelineConfig&
{
    if (binding >= vertexBindings.size())
        vertexBindings.resize(binding + 1);
    vertexBindings[binding].binding = binding;
    vertexBindings[binding].stride = stride;
    vertexBindings[binding].inputRate = inputRate;
    return *this;
}

auto PipelineConfig::withVertexBufferLayout(const VertexBufferLayout &layout) -> PipelineConfig&
{
    withVertexBinding(0, layout.getSize(), VK_VERTEX_INPUT_RATE_VERTEX);

    for (auto i = 0; i < layout.getAttributeCount(); i++)
    {
        VkFormat vkFormat;
        switch (layout.getAttribute(i).size)
        {
            case 4:
                vkFormat = VK_FORMAT_R32_SFLOAT;
                break;
            case 8:
                vkFormat = VK_FORMAT_R32G32_SFLOAT;
                break;
            case 12:
                vkFormat = VK_FORMAT_R32G32B32_SFLOAT;
                break;
            default:
                SL_PANIC("Unsupported vertex attribute size");
                break;
        }
        withVertexAttribute(i, 0, vkFormat, layout.getAttribute(i).offset);
    }

    return *this;
}

auto PipelineConfig::withDescriptorSetLayout(VkDescriptorSetLayout layout) -> PipelineConfig&
{
    descSetLayouts.push_back(layout);
    return *this;
}

auto PipelineConfig::withFrontFace(VkFrontFace frontFace) -> PipelineConfig&
{
    rasterStateInfo.frontFace = frontFace;
    return *this;
}

auto PipelineConfig::withCullMode(VkCullModeFlags cullFlags) -> PipelineConfig&
{
    rasterStateInfo.cullMode = cullFlags;
    return *this;
}

auto PipelineConfig::withDepthTest(bool write, bool test) -> PipelineConfig&
{
    depthStencilStateInfo.depthWriteEnable = write;
    depthStencilStateInfo.depthTestEnable = test;
    return *this;
}

auto PipelineConfig::withBlend(bool enabled, VkBlendFactor srcColorFactor, VkBlendFactor dstColorFactor,
    VkBlendFactor srcAlphaFactor, VkBlendFactor dstAlphaFactor) -> PipelineConfig&
{
    blendAttachmentState.blendEnable = enabled ? VK_TRUE : VK_FALSE;
    blendAttachmentState.srcColorBlendFactor = srcColorFactor;
    blendAttachmentState.dstColorBlendFactor = dstColorFactor;
    blendAttachmentState.srcAlphaBlendFactor = srcAlphaFactor;
    blendAttachmentState.dstAlphaBlendFactor = dstAlphaFactor;
    return *this;
}

#endif
