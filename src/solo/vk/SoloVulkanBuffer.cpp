/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanBuffer.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanRenderer.h"

using namespace solo;

auto VulkanBuffer::createStaging(VulkanRenderer *renderer, VkDeviceSize size, const void *initialData) -> VulkanBuffer
{
    auto buffer = VulkanBuffer(renderer, size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (initialData)
        buffer.updateAll(initialData);

    return buffer;
}

auto VulkanBuffer::createUniformHostVisible(VulkanRenderer *renderer, VkDeviceSize size) -> VulkanBuffer
{
    return VulkanBuffer(renderer, size,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

auto VulkanBuffer::createDeviceLocal(VulkanRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, const void *data) -> VulkanBuffer
{
    auto stagingBuffer = createStaging(renderer, size, data);
    auto buffer = VulkanBuffer(renderer, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usageFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    stagingBuffer.transferTo(buffer, renderer->getQueue(), renderer->getCommandPool());
    return buffer;
}

auto VulkanBuffer::createHostVisible(VulkanRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, const void *data) -> VulkanBuffer
{
    auto buffer = VulkanBuffer(renderer, size, usageFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    buffer.updateAll(data);
    return buffer;
}

VulkanBuffer::VulkanBuffer(VulkanRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memPropertyFlags):
    device(renderer->getDevice()),
    renderer(renderer),
    size(size)
{
    VkBufferCreateInfo bufferInfo {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.flags = 0;
    bufferInfo.queueFamilyIndexCount = 0;
    bufferInfo.pQueueFamilyIndices = nullptr;

    buffer = VulkanResource<VkBuffer>{device, vkDestroyBuffer};
    SL_VK_CHECK_RESULT(vkCreateBuffer(device, &bufferInfo, nullptr, buffer.cleanRef()));

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(device, buffer, &memReqs);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = vk::findMemoryType(renderer->getPhysicalMemoryFeatures(), memReqs.memoryTypeBits, memPropertyFlags);

    memory = VulkanResource<VkDeviceMemory>{device, vkFreeMemory};
    SL_VK_CHECK_RESULT(vkAllocateMemory(device, &allocInfo, nullptr, memory.cleanRef()));
    SL_VK_CHECK_RESULT(vkBindBufferMemory(device, buffer, memory, 0));
}

void VulkanBuffer::updateAll(const void *newData) const
{
    void *ptr = nullptr;
	SL_VK_CHECK_RESULT(vkMapMemory(device, memory, 0, VK_WHOLE_SIZE, 0, &ptr));
	memcpy(ptr, newData, size);
	vkUnmapMemory(device, memory);
}

void VulkanBuffer::updatePart(const void *newData, u32 offset, u32 size)
{
    void *ptr = nullptr;
	SL_VK_CHECK_RESULT(vkMapMemory(device, memory, offset, VK_WHOLE_SIZE, 0, &ptr));
	memcpy(ptr, newData, size);
	vkUnmapMemory(device, memory);
}

void VulkanBuffer::transferTo(const VulkanBuffer &dst, VkQueue queue, VkCommandPool cmdPool) const
{
	const auto cmdBuf = vk::createCommandBuffer(device, cmdPool, true);

    VkBufferCopy copyRegion{};
    copyRegion.size = dst.size;
    vkCmdCopyBuffer(cmdBuf, buffer, dst.buffer, 1, &copyRegion);

    vk::flushCommandBuffer(cmdBuf, queue);
}

#endif