/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanDescriptorPool.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;
using namespace vk;

DescriptorPool::DescriptorPool(VkDevice device, uint32_t maxSetCount, const DescriptorPoolConfig &config):
    device(device)
{
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = config.sizes.size();
    poolInfo.pPoolSizes = config.sizes.data();
    poolInfo.maxSets = maxSetCount;

    Resource<VkDescriptorPool> pool{device, vkDestroyDescriptorPool};
    SL_VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, pool.cleanRef()));

    this->pool = std::move(pool);
}

auto DescriptorPool::allocateSet(VkDescriptorSetLayout layout) const -> VkDescriptorSet
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    VkDescriptorSet set;
    SL_VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &set));

    return set;
}

auto DescriptorPoolConfig::forDescriptors(VkDescriptorType descriptorType, uint32_t descriptorCount) -> DescriptorPoolConfig&
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = descriptorType;
    poolSize.descriptorCount = descriptorCount;
    sizes.push_back(poolSize);
    return *this;
}

#endif