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
    stagingBuffer.transferTo(buffer, renderer->queue(), renderer->commandPool());
    return buffer;
}

auto VulkanBuffer::createHostVisible(VulkanRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, const void *data) -> VulkanBuffer
{
    auto buffer = VulkanBuffer(renderer, size, usageFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    buffer.updateAll(data);
    return buffer;
}

VulkanBuffer::VulkanBuffer(VulkanRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memPropertyFlags):
    device_(renderer->device()),
    renderer_(renderer),
    size_(size)
{
    VkBufferCreateInfo bufferInfo {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.flags = 0;
    bufferInfo.queueFamilyIndexCount = 0;
    bufferInfo.pQueueFamilyIndices = nullptr;

    buffer_ = VulkanResource<VkBuffer>{device_, vkDestroyBuffer};
    SL_VK_CHECK_RESULT(vkCreateBuffer(device_, &bufferInfo, nullptr, buffer_.cleanRef()));

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(device_, buffer_, &memReqs);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = vk::findMemoryType(renderer->physicalMemoryFeatures(), memReqs.memoryTypeBits, memPropertyFlags);

    memory_ = VulkanResource<VkDeviceMemory>{device_, vkFreeMemory};
    SL_VK_CHECK_RESULT(vkAllocateMemory(device_, &allocInfo, nullptr, memory_.cleanRef()));
    SL_VK_CHECK_RESULT(vkBindBufferMemory(device_, buffer_, memory_, 0));
}

void VulkanBuffer::updateAll(const void *newData) const
{
    void *ptr = nullptr;
    SL_VK_CHECK_RESULT(vkMapMemory(device_, memory_, 0, VK_WHOLE_SIZE, 0, &ptr));
    memcpy(ptr, newData, size_);
    vkUnmapMemory(device_, memory_);
}

void VulkanBuffer::updatePart(const void *newData, u32 offset, u32 size)
{
    void *ptr = nullptr;
    SL_VK_CHECK_RESULT(vkMapMemory(device_, memory_, offset, VK_WHOLE_SIZE, 0, &ptr));
    memcpy(ptr, newData, size);
    vkUnmapMemory(device_, memory_);
}

void VulkanBuffer::transferTo(const VulkanBuffer &dst, VkQueue queue, VkCommandPool cmdPool) const
{
    const auto cmdBuf = vk::createCommandBuffer(device_, cmdPool, true);

    VkBufferCopy copyRegion{};
    copyRegion.size = dst.size_;
    vkCmdCopyBuffer(cmdBuf, buffer_, dst.buffer_, 1, &copyRegion);

    vk::flushCommandBuffer(cmdBuf, queue);
}

#endif