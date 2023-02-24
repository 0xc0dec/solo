/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"

namespace solo
{
    class VulkanMaterial;

    class VulkanDescriptorSetConfig
    {
    public:
        void addUniformBuffer(u32 binding);
        void addSampler(u32 binding);

    private:
        friend class VulkanDescriptorSet;

        vec<VkDescriptorSetLayoutBinding> bindings_;
        umap<VkDescriptorType, VkDescriptorPoolSize> sizes_;
    };

    class VulkanDescriptorSet
    {
    public:
        VulkanDescriptorSet() = default;
        VulkanDescriptorSet(VkDevice device, const VulkanDescriptorSetConfig &cfg);
        VulkanDescriptorSet(VulkanDescriptorSet &&other) = default;
        VulkanDescriptorSet(const VulkanDescriptorSet &other) = delete;
        ~VulkanDescriptorSet() = default;

        auto layout() const -> VkDescriptorSetLayout
        {
            return layout_;
        }

        void updateUniformBuffer(u32 binding, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range) const;
        void updateSampler(u32 binding, VkImageView view, VkSampler sampler, VkImageLayout layout) const;

        auto operator=(const VulkanDescriptorSet &other) -> VulkanDescriptorSet & = delete;
        auto operator=(VulkanDescriptorSet &&other) -> VulkanDescriptorSet & = default;

        operator bool() const
        {
            return set_ != VK_NULL_HANDLE;
        }
        operator const VkDescriptorSet *() const
        {
            return &set_;
        }

    private:
        VkDevice device_ = VK_NULL_HANDLE;
        VulkanResource<VkDescriptorPool> pool_;
        VulkanResource<VkDescriptorSetLayout> layout_;
        VkDescriptorSet set_ = VK_NULL_HANDLE;
    };
}

#endif