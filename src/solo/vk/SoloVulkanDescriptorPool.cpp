/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanDescriptorPool.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;

VulkanDescriptorPool::VulkanDescriptorPool(VkDevice device, u32 maxSetCount, const VulkanDescriptorPoolConfig &config):
    device_(device)
{
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = config.sizes_.size();
    poolInfo.pPoolSizes = config.sizes_.data();
    poolInfo.maxSets = maxSetCount;

    VulkanResource<VkDescriptorPool> pool{device, vkDestroyDescriptorPool};
    SL_VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, pool.cleanRef()));

    this->pool_ = std::move(pool);
}

auto VulkanDescriptorPool::allocateSet(VkDescriptorSetLayout layout) const -> VkDescriptorSet
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool_;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    VkDescriptorSet set;
    SL_VK_CHECK_RESULT(vkAllocateDescriptorSets(device_, &allocInfo, &set));

    return set;
}

auto VulkanDescriptorPoolConfig::forDescriptors(VkDescriptorType descriptorType, u32 descriptorCount) -> VulkanDescriptorPoolConfig&
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = descriptorType;
    poolSize.descriptorCount = descriptorCount;
    sizes_.push_back(poolSize);
    return *this;
}

#endif