#include "SoloVulkanBuffer.h"

using namespace solo;


VulkanBuffer::VulkanBuffer(VkDevice device, VkPhysicalDeviceMemoryProperties physicalDeviceMemProps, VkDeviceSize size,
    VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
    VkBufferCreateInfo bufferInfo {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.flags = 0;
    bufferInfo.queueFamilyIndexCount = 0;
    bufferInfo.pQueueFamilyIndices = nullptr;

    SL_CHECK_VK_RESULT(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer));

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(device, buffer, &memReqs);

    VkMemoryAllocateInfo memAllocInfo {};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.allocationSize = memReqs.size;
    memAllocInfo.memoryTypeIndex = VulkanHelper::findMemoryType(physicalDeviceMemProps, memReqs.memoryTypeBits, properties);

    SL_CHECK_VK_RESULT(vkAllocateMemory(device, &memAllocInfo, nullptr, &deviceMemory));
}


VulkanBuffer::~VulkanBuffer()
{
    // TODO
}


void VulkanBuffer::setData(void* data, uint32_t size, VkCommandPool cmdPool)
{

}
