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
        class DescriptorPoolConfig
        {
        public:
            auto forDescriptors(VkDescriptorType descriptorType, u32 descriptorCount) -> DescriptorPoolConfig&;

        private:
            friend class DescriptorPool;

            vec<VkDescriptorPoolSize> sizes;
        };

        class DescriptorPool
        {
        public:
            DescriptorPool() {}
            DescriptorPool(VkDevice device, u32 maxSetCount, const DescriptorPoolConfig &config);
            DescriptorPool(const DescriptorPool &other) = delete;
            DescriptorPool(DescriptorPool &&other) = default;
            ~DescriptorPool() {}

            auto operator=(const DescriptorPool &other) -> DescriptorPool& = delete;
            auto operator=(DescriptorPool &&other) -> DescriptorPool& = default;

            auto allocateSet(VkDescriptorSetLayout layout) const -> VkDescriptorSet;

        private:
            VkDevice device = nullptr;
            Resource<VkDescriptorPool> pool;
        };
    }
}

#endif