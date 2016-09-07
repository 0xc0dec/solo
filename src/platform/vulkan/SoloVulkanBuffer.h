#pragma once

#include "SoloCommon.h"
#include "SoloVulkan.h"

namespace solo
{
    class VulkanBuffer
    {
    public:
        static VulkanBuffer create(VkDevice device, void* data, VkDeviceSize size, VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties, VkPhysicalDeviceMemoryProperties memProps);

        VulkanBuffer(VulkanBuffer &&other);
        VulkanBuffer(const VulkanBuffer &other) = delete;

        ~VulkanBuffer();

        VulkanBuffer &operator=(const VulkanBuffer &other) = delete;
        VulkanBuffer &operator=(VulkanBuffer &&other);

        VkBuffer& getHandle();
        void updateData(void* dataUpdate);

    private:
        VulkanBuffer(VkDevice device, VkBuffer buffer, VkDeviceMemory memory);
        
        void cleanup();

        VkDevice device = nullptr;
        VkBuffer buffer = nullptr;
        VkDeviceMemory deviceMemory = nullptr;
    };

    inline VkBuffer& VulkanBuffer::getHandle()
    {
        return buffer;
    }
}