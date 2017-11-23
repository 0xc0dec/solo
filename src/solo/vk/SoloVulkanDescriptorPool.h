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
        class VulkanDescriptorPoolConfig
        {
        public:
            auto forDescriptors(VkDescriptorType descriptorType, u32 descriptorCount) -> VulkanDescriptorPoolConfig&;

        private:
            friend class VulkanDescriptorPool;

            vec<VkDescriptorPoolSize> sizes;
        };

        class VulkanDescriptorPool
        {
        public:
            VulkanDescriptorPool() {}
            VulkanDescriptorPool(VkDevice device, u32 maxSetCount, const VulkanDescriptorPoolConfig &config);
            VulkanDescriptorPool(const VulkanDescriptorPool &other) = delete;
            VulkanDescriptorPool(VulkanDescriptorPool &&other) = default;
            ~VulkanDescriptorPool() {}

            auto operator=(const VulkanDescriptorPool &other) -> VulkanDescriptorPool& = delete;
            auto operator=(VulkanDescriptorPool &&other) -> VulkanDescriptorPool& = default;

            auto allocateSet(VkDescriptorSetLayout layout) const -> VkDescriptorSet;

        private:
            VkDevice device = nullptr;
            VulkanResource<VkDescriptorPool> pool;
        };
    }
}

#endif