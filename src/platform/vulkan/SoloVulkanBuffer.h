#pragma once

#include "SoloCommon.h"
#include "SoloVulkan.h"

namespace solo
{
    class VulkanBuffer
    {
    public:
        static VulkanBuffer create(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties, VkPhysicalDeviceMemoryProperties memProps);

        VulkanBuffer(VulkanBuffer &&other);
        VulkanBuffer(const VulkanBuffer &other) = delete;

        ~VulkanBuffer();

        VulkanBuffer &operator=(const VulkanBuffer &other) = delete;
        VulkanBuffer &operator=(VulkanBuffer &&other);

        VkBuffer& getHandle();

        void update(void* dataUpdate);
        void transferTo(const VulkanBuffer& other, VkQueue queue, VkCommandPool cmdPool);

    private:
        VulkanBuffer(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize size);
        
        void cleanup();

        VkDevice device = nullptr;
        VkBuffer buffer = nullptr;
        VkDeviceMemory memory = nullptr;
        VkDeviceSize size = 0;
    };

    inline VkBuffer& VulkanBuffer::getHandle()
    {
        return buffer;
    }
}