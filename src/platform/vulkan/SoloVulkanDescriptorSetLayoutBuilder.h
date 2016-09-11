#pragma once

#include "SoloVulkan.h"

namespace solo
{
    class VulkanDescriptorSetLayoutBuilder
    {
    public:
        explicit VulkanDescriptorSetLayoutBuilder(VkDevice device);

        // TODO other rule-of-five stuff

        void setBinding(uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, VkShaderStageFlagBits stageFlags);

        auto build() -> VkDescriptorSetLayout;

    private:
        VkDevice device = nullptr;
        std::vector<VkDescriptorSetLayoutBinding> bindings;
    };
}