// Note: this file is a total hackery and a work-in-progress, so avoid reading this

#include "SoloVulkanMaterial.h"
#include <tuple>
#include <array>

using namespace solo;


VkDescriptorPool VulkanMaterial::descPool = nullptr;


VkDescriptorPool createDescriptorPool(VkDevice device, const std::vector<std::tuple<VkDescriptorType, uint32_t>> typesAndCounts)
{
    std::vector<VkDescriptorPoolSize> sizes;
    for (const auto& pair : typesAndCounts)
        sizes.push_back({ std::get<0>(pair), std::get<1>(pair) });
	
    VkDescriptorPoolCreateInfo info {};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.pNext = nullptr;
	info.poolSizeCount = sizes.size();
	info.pPoolSizes = sizes.data();
	info.maxSets = 1024; // TODO to config

    VkDescriptorPool pool = nullptr;
    SL_CHECK_VK_RESULT(vkCreateDescriptorPool(device, &info, nullptr, &pool));

    return pool;
}


VulkanMaterial::VulkanMaterial(VkDevice device):
    device(device)
{
    if (descPool == nullptr)
    {
        descPool = createDescriptorPool(device,
        {
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1024 }, // TODO to config
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024 } // TODO to config
        });
    }
}


void VulkanMaterial::rebuild()
{
    VkDescriptorSetLayoutBinding setLayoutBinding1 {};
	setLayoutBinding1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	setLayoutBinding1.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	setLayoutBinding1.binding = 0;
	setLayoutBinding1.descriptorCount = 1;

    VkDescriptorSetLayoutBinding setLayoutBinding2 {};
	setLayoutBinding2.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	setLayoutBinding2.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	setLayoutBinding2.binding = 0;
	setLayoutBinding2.descriptorCount = 1;

    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = { setLayoutBinding1 };

    VkDescriptorSetLayoutCreateInfo descSetLayoutCreateInfo {};
	descSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayoutCreateInfo.pNext = nullptr;
	descSetLayoutCreateInfo.pBindings = setLayoutBindings.data();
	descSetLayoutCreateInfo.bindingCount = setLayoutBindings.size();

    VkDescriptorSetLayout descSetLayout1, descSetLayout2;
    SL_CHECK_VK_RESULT(vkCreateDescriptorSetLayout(device, &descSetLayoutCreateInfo, nullptr, &descSetLayout1));
    SL_CHECK_VK_RESULT(vkCreateDescriptorSetLayout(device, &descSetLayoutCreateInfo, nullptr, &descSetLayout2));

    VkPushConstantRange pushConstantRange = {};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = sizeof(int); // TODO !!! int is just a placeholder
	pushConstantRange.size = 0;

    std::array<VkDescriptorSetLayout, 2> setLayouts = { descSetLayout1, descSetLayout2 };
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = nullptr;
	pipelineLayoutCreateInfo.setLayoutCount = 2;
	pipelineLayoutCreateInfo.pSetLayouts = setLayouts.data();
    pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
    pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

    VkPipelineLayout pipelineLayout;
    SL_CHECK_VK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo {};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pNext = nullptr;
	descriptorSetAllocateInfo.descriptorPool = descPool;
	descriptorSetAllocateInfo.pSetLayouts = &descSetLayout1;
	descriptorSetAllocateInfo.descriptorSetCount = 1;

    // Desc set and write desc set

    VkDescriptorSet descSet;
    SL_CHECK_VK_RESULT(vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, &descSet));

    VkDescriptorImageInfo descImgInfo;

    VkWriteDescriptorSet writeDescriptorSet {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = nullptr;
	writeDescriptorSet.dstSet = descSet;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // TODO
	writeDescriptorSet.dstBinding = 0;
	writeDescriptorSet.pImageInfo = &descImgInfo;
	writeDescriptorSet.descriptorCount = 1;

    VkWriteDescriptorSet writeDescSet;
    vkUpdateDescriptorSets(device, 1, &writeDescSet, 0, nullptr);
}