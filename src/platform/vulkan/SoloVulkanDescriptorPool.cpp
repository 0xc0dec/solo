// Note: this file is a total hackery and a work-in-progress, so avoid reading to stay sane

#include "SoloVulkanDescriptorPool.h"

using namespace solo;


VulkanDescriptorPool::VulkanDescriptorPool(VkDevice device, VkDescriptorType type, uint32_t descriptorCount, uint32_t maxSetCount):
    device(device)
{
    VkDescriptorPoolSize poolSize {};
    poolSize.type = type;
    poolSize.descriptorCount = descriptorCount;

    VkDescriptorPoolCreateInfo poolInfo {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = maxSetCount;

    SL_CHECK_VK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &pool));
}


VulkanDescriptorPool::VulkanDescriptorPool(VulkanDescriptorPool&& other):
    device(std::move(other.device)),
    pool(std::move(other.pool))
{
    other.device = nullptr;
    other.pool = nullptr;
}


VulkanDescriptorPool::~VulkanDescriptorPool()
{
    cleanup();
}


auto VulkanDescriptorPool::operator=(VulkanDescriptorPool&& other) -> VulkanDescriptorPool&
{
    cleanup();

    device = std::move(other.device);
    pool = std::move(other.pool);
    other.device = nullptr;
    other.pool = nullptr;

    return *this;
}


auto VulkanDescriptorPool::allocateSet(VkDescriptorSetLayout layout) const -> VkDescriptorSet
{
    VkDescriptorSetAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    VkDescriptorSet set = nullptr;
    SL_CHECK_VK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &set));

    return set;
}


void VulkanDescriptorPool::cleanup()
{
    if (pool)
        vkDestroyDescriptorPool(device, pool, nullptr);
}
