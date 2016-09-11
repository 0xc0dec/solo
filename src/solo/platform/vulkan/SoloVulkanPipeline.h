// Note: this file is a total hackery and a work-in-progress, so avoid reading to stay sane

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

#pragma once

#include "SoloCommon.h"
#include "SoloVulkan.h"

namespace solo
{
    class VulkanPipeline
    {
    public:
        SL_NONCOPYABLE(VulkanPipeline)
        VulkanPipeline(VkDevice device, VkRenderPass renderPass);
        ~VulkanPipeline();

        VkPipeline getHandle() const;
        VkPipelineLayout getLayoutHandle() const;

        void setVertexShader(VkShaderModule shader, const char* entryPoint);
        void setFragmentShader(VkShaderModule shader, const char* entryPoint);

        void setVertexAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset);
        void resetVertexAttributes();

        void setVertexSize(uint32_t size);

        void setTopology(VkPrimitiveTopology topology);

        void setCullMode(VkCullModeFlags mode);
        void setFrontFace(VkFrontFace frontFace);

        void setRasterizationSampleCount(VkSampleCountFlagBits sampleCount);

        void setColorBlend(VkBlendFactor srcFactor, VkBlendFactor dstFactor, VkBlendOp operation);
        void setAlphaBlend(VkBlendFactor srcFactor, VkBlendFactor dstFactor, VkBlendOp operation);
        void setColorWriteMask(VkColorComponentFlags mask);

        void setDescriptorSetLayouts(VkDescriptorSetLayout* layouts, uint32_t count);

        void rebuild();

        void bind(VkCommandBuffer cmdBuf);

    private:
        VkDevice device = nullptr;
        VkRenderPass renderPass = nullptr;
        VkPipeline pipeline = nullptr;
        VkPipelineLayout layout = nullptr;

        VkShaderModule vertexShader = nullptr;
        VkShaderModule fragmentShader = nullptr;
        VkPipelineShaderStageCreateInfo vertexShaderStageInfo;
        VkPipelineShaderStageCreateInfo fragmentShaderStageInfo;

        std::vector<VkVertexInputAttributeDescription> vertexAttrs;
        std::vector<VkDescriptorSetLayout> descSetLayouts;

        uint32_t vertexSize = 0;
        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        VkPipelineRasterizationStateCreateInfo rasterState;
        VkPipelineMultisampleStateCreateInfo multisampleState;
        VkPipelineColorBlendAttachmentState blendAttachmentState;
        VkPipelineColorBlendStateCreateInfo colorBlendState;
    };

    inline VkPipeline VulkanPipeline::getHandle() const
    {
        return pipeline;
    }

    inline VkPipelineLayout VulkanPipeline::getLayoutHandle() const
    {
        return layout;
    }

    inline void VulkanPipeline::setTopology(VkPrimitiveTopology topology)
    {
        this->topology = topology;
    }

    inline void VulkanPipeline::setVertexSize(uint32_t size)
    {
        vertexSize = size;
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

    inline void VulkanPipeline::setColorBlend(VkBlendFactor srcFactor, VkBlendFactor dstFactor, VkBlendOp operation)
    {
        blendAttachmentState.srcColorBlendFactor = srcFactor;
        blendAttachmentState.dstColorBlendFactor = dstFactor;
        blendAttachmentState.colorBlendOp = operation;
    }

    inline void VulkanPipeline::setAlphaBlend(VkBlendFactor srcFactor, VkBlendFactor dstFactor, VkBlendOp operation)
    {
        blendAttachmentState.srcAlphaBlendFactor = srcFactor;
        blendAttachmentState.dstAlphaBlendFactor = dstFactor;
        blendAttachmentState.alphaBlendOp = operation;
    }

    inline void VulkanPipeline::setColorWriteMask(VkColorComponentFlags mask)
    {
        blendAttachmentState.colorWriteMask = mask;
    }
}