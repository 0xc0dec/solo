#include "SoloVulkanMaterial.h"
#include <tuple>

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


VulkanMaterial::VulkanMaterial(VkDevice device)
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
