#pragma once

#include "SoloCommon.h"
#include "SoloVulkan.h"

namespace solo
{
    class VulkanPipeline
    {
    public:
        VulkanPipeline(VkDevice device);
        ~VulkanPipeline();
        SL_NONCOPYABLE(VulkanPipeline)

        void setVertexShader(VkShaderModule shader, const char* entryPoint);
        void setFragmentShader(VkShaderModule shader, const char* entryPoint);

        void rebuild();

    private:
        VkDevice device = nullptr;
        VkPipeline pipeline = nullptr;

        VkShaderModule vertexShader = nullptr;
        VkShaderModule fragmentShader = nullptr;
        VkPipelineShaderStageCreateInfo vertexShaderStageInfo;
        VkPipelineShaderStageCreateInfo fragmentShaderStageInfo;
    };
}