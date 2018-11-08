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
    class VulkanRenderer;

    class VulkanBuffer
    {
    public:
        static auto staging(VulkanRenderer *renderer, VkDeviceSize size, const void *initialData = nullptr) -> VulkanBuffer;
        static auto uniformHostVisible(VulkanRenderer *renderer, VkDeviceSize size) -> VulkanBuffer;
        static auto deviceLocal(VulkanRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, const void *data) -> VulkanBuffer;
        static auto hostVisible(VulkanRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, const void *data) -> VulkanBuffer;

        VulkanBuffer() = default;
        VulkanBuffer(VulkanRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memPropertyFlags);
        VulkanBuffer(VulkanBuffer &&other) = default;
        VulkanBuffer(const VulkanBuffer &other) = delete;
        ~VulkanBuffer() = default;

        auto operator=(const VulkanBuffer &other) -> VulkanBuffer& = delete;
        auto operator=(VulkanBuffer &&other) -> VulkanBuffer& = default;

        operator VkBuffer() { return buffer_; }

        auto handle() const -> VkBuffer { return buffer_; }

        void updateAll(const void *newData) const;
        void updatePart(const void *newData, u32 offset, u32 size);
        void transferTo(const VulkanBuffer& other, VkQueue queue, VkCommandPool cmdPool) const;

    private:
        VkDevice device_ = nullptr;
        VulkanResource<VkDeviceMemory> memory_;
        VulkanResource<VkBuffer> buffer_;
        VkDeviceSize size_ = 0;
    };
}

#endif