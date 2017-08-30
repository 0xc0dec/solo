/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanDescriptorPool.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;
using namespace vk;


DescriptorPool::DescriptorPool(VkDevice device, VkDescriptorType type, uint32_t descriptorCount, uint32_t maxSetCount):
    device(device)
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = type;
    poolSize.descriptorCount = descriptorCount;

    VkDescriptorPoolCreateInfo poolInfo {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = maxSetCount;

    this->pool = Resource<VkDescriptorPool>{device, vkDestroyDescriptorPool};
    SL_VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, pool.cleanAndExpose()));
}


DescriptorPool::DescriptorPool(DescriptorPool &&other) noexcept
{
    swap(other);
}


auto DescriptorPool::operator=(DescriptorPool other) noexcept -> DescriptorPool&
{
    swap(other);
    return *this;
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


void DescriptorPool::swap(DescriptorPool &other) noexcept
{
    std::swap(device, other.device);
    std::swap(pool, other.pool);
}


#endif