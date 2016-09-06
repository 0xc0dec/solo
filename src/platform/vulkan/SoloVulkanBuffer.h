#pragma once

#include "SoloCommon.h"
#include "SoloVulkan.h"

namespace solo
{
    class VulkanBuffer
    {
    public:
        VulkanBuffer(VkDevice device, VkPhysicalDeviceMemoryProperties physicalDeviceMemProps, VkDeviceSize size,
            VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
        ~VulkanBuffer();

        SL_NONCOPYABLE(VulkanBuffer)

        void setData(void* data, uint32_t size, VkCommandPool cmdPool);

    private:
        VkBuffer buffer = nullptr;
        VkDeviceMemory deviceMemory = nullptr;
    };
}