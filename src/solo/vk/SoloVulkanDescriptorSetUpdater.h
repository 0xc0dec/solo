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
        class DescriptorSetUpdater
        {
        public:
            explicit DescriptorSetUpdater(VkDevice device);

            auto forUniformBuffer(u32 binding, VkDescriptorSet set, VkBuffer buffer, VkDeviceSize offset,
                VkDeviceSize range) -> DescriptorSetUpdater&;
            auto forTexture(u32 binding, VkDescriptorSet set, VkImageView view, VkSampler sampler,
                VkImageLayout layout) -> DescriptorSetUpdater&;

            void updateSets();

        private:
            struct Item
            {
                VkDescriptorBufferInfo buffer;
                VkDescriptorImageInfo image;
                u32 binding;
                VkDescriptorSet targetSet;
            };

            VkDevice device = nullptr;
            vec<Item> items;
        };
    }
}

#endif
