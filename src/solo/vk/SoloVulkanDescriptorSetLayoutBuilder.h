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
        class DescriptorSetLayoutBuilder
        {
        public:
            explicit DescriptorSetLayoutBuilder(VkDevice device);

            auto withBinding(uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount,
                VkShaderStageFlagBits stageFlags) -> DescriptorSetLayoutBuilder&;

            auto build() -> Resource<VkDescriptorSetLayout>;

        private:
            VkDevice device = nullptr;
            std::vector<VkDescriptorSetLayoutBinding> bindings;
        };
    }
}

#endif