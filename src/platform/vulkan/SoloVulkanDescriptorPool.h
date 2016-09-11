// Note: this file is a total hackery and a work-in-progress, so avoid reading to stay sane

#pragma once

#include "SoloVulkan.h"

namespace solo
{
    // TODO currently supports pools that allocate only one predefined type of sets
    class VulkanDescriptorPool
    {
    public:
        VulkanDescriptorPool() {}
        VulkanDescriptorPool(VkDevice device, VkDescriptorType type, uint32_t descriptorCount, uint32_t maxSetCount);
        VulkanDescriptorPool(const VulkanDescriptorPool& other) = delete;
        VulkanDescriptorPool(VulkanDescriptorPool&& other);

        ~VulkanDescriptorPool();

        auto operator=(const VulkanDescriptorPool& other) -> VulkanDescriptorPool& = delete;
        auto operator=(VulkanDescriptorPool&& other) -> VulkanDescriptorPool&;

        auto allocateSet(VkDescriptorSetLayout layout) const -> VkDescriptorSet;

    private:
        void cleanup();

        VkDevice device = nullptr;
        VkDescriptorPool pool = nullptr;
    };
}