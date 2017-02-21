/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloVulkanPipeline.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;
using namespace vk;


Pipeline::Pipeline(VkDevice device, VkRenderPass renderPass, Resource<VkPipeline> &&pipeline, Resource<VkPipelineLayout> &&layout):
    device(device),
    renderPass(renderPass),
    pipeline(std::move(pipeline)),
    layout(std::move(layout))
{
}


Pipeline::Pipeline(Pipeline &&other) noexcept
{
    swap(other);
}


auto Pipeline::operator=(Pipeline other) noexcept -> Pipeline&
{
    swap(other);
    return *this;
}


void Pipeline::swap(Pipeline &other) noexcept
{
    std::swap(device, other.device);
    std::swap(renderPass, other.renderPass);
    std::swap(pipeline, other.pipeline);
    std::swap(layout, other.layout);
}


PipelineBuilder::PipelineBuilder(VkDevice device, VkRenderPass renderPass):
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


auto PipelineBuilder::withVertexShader(VkShaderModule shader, const char *entryPoint) -> PipelineBuilder&
{
    vertexShader = shader;
    if (vertexShader)
        vertexShaderStageInfo = createShaderStageInfo(true, vertexShader, entryPoint);
    return *this;
}


auto PipelineBuilder::withFragmentShader(VkShaderModule shader, const char *entryPoint) -> PipelineBuilder&
{
    fragmentShader = shader;
    if (fragmentShader)
        fragmentShaderStageInfo = createShaderStageInfo(false, fragmentShader, entryPoint);
    return *this;
}


auto PipelineBuilder::withColorBlend(VkBlendFactor srcFactor, VkBlendFactor dstFactor, VkBlendOp operation) -> PipelineBuilder&
{
    blendAttachmentState.srcColorBlendFactor = srcFactor;
    blendAttachmentState.dstColorBlendFactor = dstFactor;
    blendAttachmentState.colorBlendOp = operation;
    return *this;
}


auto PipelineBuilder::withAlphaBlend(VkBlendFactor srcFactor, VkBlendFactor dstFactor, VkBlendOp operation) -> PipelineBuilder&
{
    blendAttachmentState.srcAlphaBlendFactor = srcFactor;
    blendAttachmentState.dstAlphaBlendFactor = dstFactor;
    blendAttachmentState.alphaBlendOp = operation;
    return *this;
}


auto PipelineBuilder::withVertexAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset) -> PipelineBuilder&
{
    if (location >= vertexAttrs.size())
        vertexAttrs.resize(location + 1);
    vertexAttrs[location].location = location;
    vertexAttrs[location].binding = binding;
    vertexAttrs[location].format = format;
    vertexAttrs[location].offset = offset;
    return *this;
}


auto PipelineBuilder::withDescriptorSetLayouts(VkDescriptorSetLayout *layouts, uint32_t count) -> PipelineBuilder&
{
    descSetLayouts.resize(count);
    for (auto i = 0; i < count; i++)
        descSetLayouts[i] = layouts[i];
    return *this;
}


auto PipelineBuilder::build() -> Pipeline
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = 0;
    pipelineLayoutInfo.setLayoutCount = descSetLayouts.size();
    pipelineLayoutInfo.pSetLayouts = descSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    Resource<VkPipelineLayout> layout{device, vkDestroyPipelineLayout};
    SL_VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, layout.replace()));

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

    Resource<VkPipeline> pipeline{device, vkDestroyPipeline};
    SL_VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, pipeline.replace()));

    return Pipeline(device, renderPass, std::move(pipeline), std::move(layout));
}


#endif
