// Note: this file is a total hackery and a work-in-progress, so avoid reading to stay sane

#include "SoloVulkanDescriptorSetLayoutBuilder.h"

using namespace solo;


VulkanDescriptorSetLayoutBuilder::VulkanDescriptorSetLayoutBuilder(VkDevice device):
    device(device)
{
}


void VulkanDescriptorSetLayoutBuilder::setBinding(uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount,
    VkShaderStageFlagBits stageFlags)
{
    if (binding >= bindings.size())
        bindings.resize(binding + 1);
    bindings[binding].binding = binding;
    bindings[binding].descriptorType = descriptorType;
    bindings[binding].descriptorCount = descriptorCount;
    bindings[binding].stageFlags = stageFlags;
    bindings[binding].pImmutableSamplers = nullptr;
}


auto VulkanDescriptorSetLayoutBuilder::build() -> VkDescriptorSetLayout
{
    VkDescriptorSetLayoutCreateInfo layoutInfo {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindings.size();
    layoutInfo.pBindings = bindings.data();

    VkDescriptorSetLayout result;
    SL_CHECK_VK_RESULT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &result));
    return result;
}