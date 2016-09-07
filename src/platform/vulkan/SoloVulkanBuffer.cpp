#include "SoloVulkanBuffer.h"

using namespace solo;


VulkanBuffer::VulkanBuffer(VulkanBuffer&& other):
    device(other.device),
    buffer(other.buffer),
    deviceMemory(other.deviceMemory)
{
    other.device = nullptr;
    other.deviceMemory = nullptr;
    other.buffer = nullptr;
}


VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other)
{
    device = other.device;
    deviceMemory = other.deviceMemory;
    buffer = other.buffer;

    other.device = nullptr;
    other.deviceMemory = nullptr;
    other.buffer = nullptr;

    return *this;
}


VulkanBuffer::~VulkanBuffer()
{
    cleanup();
}


VulkanBuffer VulkanBuffer::create(VkDevice device, void* data, VkDeviceSize size, VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties, VkPhysicalDeviceMemoryProperties memProps)
{
    VkBufferCreateInfo bufferInfo {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.flags = 0;
    bufferInfo.queueFamilyIndexCount = 0;
    bufferInfo.pQueueFamilyIndices = nullptr;

    VkBuffer buffer = nullptr;
    SL_CHECK_VK_RESULT(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer));

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(device, buffer, &memReqs);

    VkMemoryAllocateInfo memAllocInfo {};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.allocationSize = memReqs.size;
    memAllocInfo.memoryTypeIndex = VulkanHelper::findMemoryType(memProps, memReqs.memoryTypeBits, properties);

    VkDeviceMemory memory = nullptr;
    SL_CHECK_VK_RESULT(vkAllocateMemory(device, &memAllocInfo, nullptr, &memory));

    SL_CHECK_VK_RESULT(vkBindBufferMemory(device, buffer, memory, 0));

    void* ptr = nullptr;
	SL_CHECK_VK_RESULT(vkMapMemory(device, memory, 0, VK_WHOLE_SIZE, 0, &data));
	memcpy(ptr, data, size);
	vkUnmapMemory(device, memory);

    return VulkanBuffer(device, buffer, memory);
}


VulkanBuffer::VulkanBuffer(VkDevice device, VkBuffer buffer, VkDeviceMemory memory):
    device(device),
    buffer(buffer),
    deviceMemory(memory)
{
}


void VulkanBuffer::updateData(void* dataUpdate)
{
    // TODO
}


void VulkanBuffer::cleanup()
{
    if (deviceMemory)
    {
        vkFreeMemory(device, deviceMemory, nullptr);
        deviceMemory = nullptr;
    }

    if (buffer)
    {
        vkDestroyBuffer(device, buffer, nullptr);
        buffer = nullptr;
    }
}
