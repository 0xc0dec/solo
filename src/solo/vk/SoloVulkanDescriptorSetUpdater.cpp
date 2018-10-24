/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanDescriptorSetUpdater.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;

VulkanDescriptorSetUpdater::VulkanDescriptorSetUpdater(VkDevice device):
    device_(device)
{
}

auto VulkanDescriptorSetUpdater::forUniformBuffer(u32 binding, VkDescriptorSet set, VkBuffer buffer,
    VkDeviceSize offset, VkDeviceSize range) -> VulkanDescriptorSetUpdater&
{
    items_.push_back({{buffer, offset, range}, {}, binding, set});
    return *this;
}

auto VulkanDescriptorSetUpdater::forImageSampler(u32 binding, VkDescriptorSet set, VkImageView view,
    VkSampler sampler, VkImageLayout layout) -> VulkanDescriptorSetUpdater&
{
    items_.push_back({{}, {sampler, view, layout}, binding, set});
    return *this;
}

void VulkanDescriptorSetUpdater::updateSets()
{
    vec<VkWriteDescriptorSet> writes;

    for (const auto &item: items_)
    {
        const auto bufferInfo = item.buffer.buffer ? &item.buffer : nullptr;
        const auto imageInfo = item.image.imageView ? &item.image : nullptr;
        const auto type = bufferInfo ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = item.targetSet;
        write.dstBinding = item.binding;
        write.dstArrayElement = 0;
        write.descriptorType = type;
        write.descriptorCount = 1;
        write.pBufferInfo = bufferInfo;
        write.pImageInfo = imageInfo;
        write.pTexelBufferView = nullptr;
        writes.push_back(write);
    }

    vkUpdateDescriptorSets(device_, writes.size(), writes.data(), 0, nullptr);
}

#endif
