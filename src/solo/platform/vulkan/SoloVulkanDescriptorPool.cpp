/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloVulkanDescriptorPool.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;


VulkanDescriptorPool::VulkanDescriptorPool(VkDevice device, VkDescriptorType type, uint32_t descriptorCount, uint32_t maxSetCount):
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

    SL_CHECK_VK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &pool));
}


VulkanDescriptorPool::VulkanDescriptorPool(VulkanDescriptorPool&& other) noexcept:
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


auto VulkanDescriptorPool::operator=(VulkanDescriptorPool&& other) noexcept -> VulkanDescriptorPool&
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
    VkDescriptorSetAllocateInfo allocInfo{};
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
    pool = nullptr;
}

#endif