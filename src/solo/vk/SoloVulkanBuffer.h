/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"

namespace solo
{
    class VulkanDevice;

    class VulkanBuffer
    {
    public:
        static auto staging(const VulkanDevice &dev, VkDeviceSize size, const void *initialData = nullptr) -> VulkanBuffer;
        static auto uniformHostVisible(const VulkanDevice &dev, VkDeviceSize size) -> VulkanBuffer;
        static auto deviceLocal(const VulkanDevice &dev, VkDeviceSize size, VkBufferUsageFlags usageFlags, const void *data) -> VulkanBuffer;
        static auto hostVisible(const VulkanDevice &dev, VkDeviceSize size, VkBufferUsageFlags usageFlags, const void *data) -> VulkanBuffer;

        VulkanBuffer() = default;
        VulkanBuffer(const VulkanDevice &dev, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memPropertyFlags);
        VulkanBuffer(VulkanBuffer &&other) = default;
        VulkanBuffer(const VulkanBuffer &other) = delete;
        ~VulkanBuffer() = default;

        auto operator=(const VulkanBuffer &other) -> VulkanBuffer& = delete;
        auto operator=(VulkanBuffer &&other) -> VulkanBuffer& = default;

        operator VkBuffer() { return buffer_; }

        auto handle() const -> VkBuffer { return buffer_; }
        auto size() const -> VkDeviceSize { return size_; }

        void updateAll(const void *newData) const;
        void updatePart(const void *newData, u32 offset, u32 size);
        void transferTo(const VulkanBuffer& dst) const;

    private:
        const VulkanDevice *device_ = nullptr;
        VulkanResource<VkDeviceMemory> memory_;
        VulkanResource<VkBuffer> buffer_;
        VkDeviceSize size_ = 0;
    };
}

#endif