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

            auto withBinding(u32 binding, VkDescriptorType descriptorType, u32 descriptorCount,
                VkShaderStageFlagBits stageFlags) -> DescriptorSetLayoutBuilder&;

            auto build() -> Resource<VkDescriptorSetLayout>;

        private:
            VkDevice device = nullptr;
            vec<VkDescriptorSetLayoutBinding> bindings;
        };
    }
}

#endif