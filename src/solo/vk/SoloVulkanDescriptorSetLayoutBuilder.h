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
    class VulkanDescriptorSetLayoutBuilder
    {
    public:
        explicit VulkanDescriptorSetLayoutBuilder(VkDevice device);

        auto withBinding(u32 binding, VkDescriptorType descriptorType, u32 descriptorCount,
            VkShaderStageFlagBits stageFlags) -> VulkanDescriptorSetLayoutBuilder&;

        auto build() -> VulkanResource<VkDescriptorSetLayout>;

    private:
        VkDevice device = nullptr;
        vec<VkDescriptorSetLayoutBinding> bindings;
    };
}

#endif