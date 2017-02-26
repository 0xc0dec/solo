/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"

namespace solo
{
    namespace vk
    {
        class Pipeline
        {
        public:
            Pipeline() {}
            Pipeline(VkDevice device, VkRenderPass renderPass, Resource<VkPipeline> pipeline, Resource<VkPipelineLayout> layout);
            Pipeline(Pipeline &&other) noexcept;
            Pipeline(const Pipeline &other) = delete;
            ~Pipeline() {}

            auto operator=(Pipeline other) noexcept -> Pipeline&;

            operator VkPipeline()
            {
                return pipeline;
            }

            VkPipeline getHandle() const;
            VkPipelineLayout getLayout() const;

        private:
            VkDevice device = nullptr;
            VkRenderPass renderPass = nullptr;
            Resource<VkPipeline> pipeline;
            Resource<VkPipelineLayout> layout;

            void swap(Pipeline &other) noexcept;
        };

        class PipelineBuilder
        {
        public:
            PipelineBuilder(VkDevice device, VkRenderPass renderPass, VkShaderModule vertexShader, VkShaderModule fragmentShader);
            ~PipelineBuilder() {}

            auto withTopology(VkPrimitiveTopology topology) -> PipelineBuilder&;
            auto withVertexSize(uint32_t size) -> PipelineBuilder&;

            auto withVertexAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset) -> PipelineBuilder&;
            auto withVertexBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate) -> PipelineBuilder&;

            auto withDescriptorSetLayouts(VkDescriptorSetLayout* layouts, uint32_t count) -> PipelineBuilder&;

            auto build() -> Pipeline;

        private:
            VkDevice device = nullptr;
            VkRenderPass renderPass = nullptr;

            VkShaderModule vertexShader;
            VkShaderModule fragmentShader;
            VkPipelineShaderStageCreateInfo vertexShaderStageInfo;
            VkPipelineShaderStageCreateInfo fragmentShaderStageInfo;

            std::vector<VkVertexInputAttributeDescription> vertexAttrs;
            std::vector<VkVertexInputBindingDescription> vertexBindings;
            std::vector<VkDescriptorSetLayout> descSetLayouts;

            uint32_t vertexSize = 0;
            VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        };
    
        inline VkPipeline Pipeline::getHandle() const
        {
            return pipeline;
        }

        inline VkPipelineLayout Pipeline::getLayout() const
        {
            return layout;
        }

        inline auto PipelineBuilder::withTopology(VkPrimitiveTopology topology) -> PipelineBuilder&
        {
            this->topology = topology;
            return *this;
        }

        inline auto PipelineBuilder::withVertexSize(uint32_t size) -> PipelineBuilder&
        {
            vertexSize = size;
            return *this;
        }
    }
}

#endif