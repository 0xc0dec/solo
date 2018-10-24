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
    class VulkanDescriptorPoolConfig
    {
    public:
        auto forDescriptors(VkDescriptorType descriptorType, u32 descriptorCount) -> VulkanDescriptorPoolConfig&;

    private:
        friend class VulkanDescriptorPool;

        vec<VkDescriptorPoolSize> sizes_;
    };

    class VulkanDescriptorPool
    {
    public:
        VulkanDescriptorPool() = default;
        VulkanDescriptorPool(VkDevice device, u32 maxSetCount, const VulkanDescriptorPoolConfig &config);
        VulkanDescriptorPool(const VulkanDescriptorPool &other) = delete;
        VulkanDescriptorPool(VulkanDescriptorPool &&other) = default;
        ~VulkanDescriptorPool() = default;

        auto operator=(const VulkanDescriptorPool &other) -> VulkanDescriptorPool& = delete;
        auto operator=(VulkanDescriptorPool &&other) -> VulkanDescriptorPool& = default;

        auto allocateSet(VkDescriptorSetLayout layout) const -> VkDescriptorSet;

    private:
        VkDevice device_ = nullptr;
        VulkanResource<VkDescriptorPool> pool_;
    };
}

#endif