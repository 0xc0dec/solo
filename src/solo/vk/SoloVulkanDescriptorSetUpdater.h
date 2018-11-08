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
    class VulkanDescriptorSetUpdater
    {
    public:
        explicit VulkanDescriptorSetUpdater(VkDevice device);

        auto forUniformBuffer(u32 binding, VkDescriptorSet set, VkBuffer buffer, VkDeviceSize offset,
            VkDeviceSize range) -> VulkanDescriptorSetUpdater&;
        auto forImageSampler(u32 binding, VkDescriptorSet set, VkImageView view, VkSampler sampler,
            VkImageLayout layout) -> VulkanDescriptorSetUpdater&;

        void updateSets();

    private:
        struct Item
        {
            VkDescriptorBufferInfo buffer;
            VkDescriptorImageInfo image;
            u32 binding;
            VkDescriptorSet targetSet;
        };

        VkDevice device_ = nullptr;
        vec<Item> items_;
    };
}

#endif
