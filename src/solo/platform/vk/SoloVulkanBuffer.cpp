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

#ifdef SL_VULKAN_RENDERER

using namespace solo;
using namespace vk;


Buffer::Buffer(VkDevice device, VkDeviceSize size, uint32_t flags, VkPhysicalDeviceMemoryProperties memProps):
    device(device),
    size(size)
{
    VkBufferUsageFlags usage = 0;
    VkMemoryPropertyFlags propFlags = 0;

    if (flags & Host)
        propFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    if (flags & Device)
        propFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    if (flags & Vertex)
        usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if (flags & Uniform)
        usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    if (flags & TransferSrc)
        usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    if (flags & TransferDst)
        usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    VkBufferCreateInfo bufferInfo {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.flags = 0;
    bufferInfo.queueFamilyIndexCount = 0;
    bufferInfo.pQueueFamilyIndices = nullptr;

    buffer = Resource<VkBuffer>{device, vkDestroyBuffer};
    SL_VK_CHECK_RESULT(vkCreateBuffer(device, &bufferInfo, nullptr, buffer.replace()));

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(device, buffer, &memReqs);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = findMemoryType(memProps, memReqs.memoryTypeBits, propFlags);

    memory = Resource<VkDeviceMemory>{device, vkFreeMemory};
    SL_VK_CHECK_RESULT(vkAllocateMemory(device, &allocInfo, nullptr, memory.replace()));
    SL_VK_CHECK_RESULT(vkBindBufferMemory(device, buffer, memory, 0));
}


Buffer::Buffer(Buffer &&other) noexcept
{
    swap(other);
}


auto Buffer::operator=(Buffer other) noexcept -> Buffer&
{
    swap(other);
    return *this;
}


void Buffer::update(void *newData) const
{
    void *ptr = nullptr;
	SL_VK_CHECK_RESULT(vkMapMemory(device, memory, 0, VK_WHOLE_SIZE, 0, &ptr));
	memcpy(ptr, newData, size);
	vkUnmapMemory(device, memory);
}


void Buffer::transferTo(const Buffer &dst, VkQueue queue, VkCommandPool cmdPool) const
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = cmdPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer cmdBuf;
    SL_VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, &cmdBuf));

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    SL_VK_CHECK_RESULT(vkBeginCommandBuffer(cmdBuf, &beginInfo));

    VkBufferCopy copyRegion{};
    copyRegion.size = dst.size;
    vkCmdCopyBuffer(cmdBuf, buffer, dst.buffer, 1, &copyRegion);

    SL_VK_CHECK_RESULT(vkEndCommandBuffer(cmdBuf));

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuf;

    SL_VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, nullptr));
    SL_VK_CHECK_RESULT(vkQueueWaitIdle(queue));

    vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuf);
}


void Buffer::swap(Buffer &other) noexcept
{
    std::swap(device, other.device);
    std::swap(memory, other.memory);
    std::swap(buffer, other.buffer);
    std::swap(size, other.size);
}


#endif