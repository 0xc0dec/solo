// Note: this file is a total hackery and a work-in-progress, so avoid reading to stay sane

/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloVulkanBuffer.h"

using namespace solo;


VulkanBuffer::VulkanBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
    VkPhysicalDeviceMemoryProperties memProps):
    device(device),
    size(size)
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

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = VulkanHelper::findMemoryType(memProps, memReqs.memoryTypeBits, properties);

    SL_CHECK_VK_RESULT(vkAllocateMemory(device, &allocInfo, nullptr, &memory));

    SL_CHECK_VK_RESULT(vkBindBufferMemory(device, buffer, memory, 0));
}


VulkanBuffer::VulkanBuffer(VulkanBuffer&& other):
    device(other.device),
    buffer(other.buffer),
    memory(other.memory),
    size(other.size)
{
    other.device = nullptr;
    other.memory = nullptr;
    other.buffer = nullptr;
    other.size = 0;
}


VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other)
{
    device = other.device;
    memory = other.memory;
    buffer = other.buffer;
    size = other.size;

    other.device = nullptr;
    other.memory = nullptr;
    other.buffer = nullptr;
    other.size = 0;

    return *this;
}


VulkanBuffer::~VulkanBuffer()
{
    cleanup();
}


void VulkanBuffer::update(void* newData)
{
    void* ptr = nullptr;
	SL_CHECK_VK_RESULT(vkMapMemory(device, memory, 0, VK_WHOLE_SIZE, 0, &ptr));
	memcpy(ptr, newData, size);
	vkUnmapMemory(device, memory);
}


void VulkanBuffer::transferTo(const VulkanBuffer& dst, VkQueue queue, VkCommandPool cmdPool)
{
    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = cmdPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer cmdBuf;
    SL_CHECK_VK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, &cmdBuf));

    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    SL_CHECK_VK_RESULT(vkBeginCommandBuffer(cmdBuf, &beginInfo));

    VkBufferCopy copyRegion {};
    copyRegion.size = dst.size;
    vkCmdCopyBuffer(cmdBuf, buffer, dst.buffer, 1, &copyRegion);

    SL_CHECK_VK_RESULT(vkEndCommandBuffer(cmdBuf));

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuf;

    SL_CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, nullptr));
    SL_CHECK_VK_RESULT(vkQueueWaitIdle(queue));

    vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuf);
}


void VulkanBuffer::cleanup()
{
    if (memory)
    {
        vkFreeMemory(device, memory, nullptr);
        memory = nullptr;
    }

    if (buffer)
    {
        vkDestroyBuffer(device, buffer, nullptr);
        buffer = nullptr;
    }
}
