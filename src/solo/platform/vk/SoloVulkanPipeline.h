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
            PipelineBuilder(VkDevice device, VkRenderPass renderPass);
            ~PipelineBuilder() {}

            auto withVertexShader(VkShaderModule shader, const char *entryPoint) -> PipelineBuilder&;
            auto withFragmentShader(VkShaderModule shader, const char *entryPoint) -> PipelineBuilder&;

            auto withTopology(VkPrimitiveTopology topology) -> PipelineBuilder&;
            auto withVertexSize(uint32_t size) -> PipelineBuilder&;

            auto withVertexAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset) -> PipelineBuilder&;

            auto withDescriptorSetLayouts(VkDescriptorSetLayout* layouts, uint32_t count) -> PipelineBuilder&;

            auto build() -> Pipeline;

        private:
            VkDevice device = nullptr;
            VkRenderPass renderPass = nullptr;

            VkShaderModule vertexShader{};
            VkShaderModule fragmentShader{};
            VkPipelineShaderStageCreateInfo vertexShaderStageInfo;
            VkPipelineShaderStageCreateInfo fragmentShaderStageInfo;

            std::vector<VkVertexInputAttributeDescription> vertexAttrs;
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