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
        // TODO currently supports pools that allocate only one predefined type of sets
        class DescriptorPool
        {
        public:
            DescriptorPool() {}
            DescriptorPool(VkDevice device, VkDescriptorType type, uint32_t descriptorCount, uint32_t maxSetCount);
            DescriptorPool(const DescriptorPool &other) = delete;
            DescriptorPool(DescriptorPool &&other) noexcept;

            ~DescriptorPool() {}

            auto operator=(DescriptorPool other) noexcept -> DescriptorPool&;

            auto allocateSet(VkDescriptorSetLayout layout) const -> VkDescriptorSet;

        private:
            VkDevice device = nullptr;
            Resource<VkDescriptorPool> pool;

            void swap(DescriptorPool &other) noexcept;
        };
    }
}

#endif