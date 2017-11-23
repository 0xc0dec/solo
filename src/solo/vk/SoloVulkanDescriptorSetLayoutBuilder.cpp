/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanDescriptorSetLayoutBuilder.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;

VulkanDescriptorSetLayoutBuilder::VulkanDescriptorSetLayoutBuilder(VkDevice device):
    device(device)
{
}

auto VulkanDescriptorSetLayoutBuilder::withBinding(u32 binding, VkDescriptorType descriptorType, u32 descriptorCount,
    VkShaderStageFlagBits stageFlags) -> VulkanDescriptorSetLayoutBuilder&
{
    if (binding >= bindings.size())
        bindings.resize(binding + 1); // TODO Fix this. Causes troubles when there are gaps between binding numbers

    bindings[binding].binding = binding;
    bindings[binding].descriptorType = descriptorType;
    bindings[binding].descriptorCount = descriptorCount;
    bindings[binding].stageFlags = stageFlags;
    bindings[binding].pImmutableSamplers = nullptr;

    return *this;
}

auto VulkanDescriptorSetLayoutBuilder::build() -> VulkanResource<VkDescriptorSetLayout>
{
    VkDescriptorSetLayoutCreateInfo layoutInfo {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindings.size();
    layoutInfo.pBindings = bindings.data();

    VulkanResource<VkDescriptorSetLayout> result{device, vkDestroyDescriptorSetLayout};
    SL_VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, result.cleanRef()));
    
    return result;
}

#endif