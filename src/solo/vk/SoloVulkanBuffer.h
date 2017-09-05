/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkan.h"

namespace solo
{
    namespace vk
    {
        class Renderer;

        class Buffer
        {
        public:
            static auto createStaging(Renderer *renderer, VkDeviceSize size, const void *initialData = nullptr) -> Buffer;
            static auto createUniformHostVisible(Renderer *renderer, VkDeviceSize size) -> Buffer;
            static auto createDeviceLocal(Renderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, const void *data) -> Buffer;

            Buffer() {}
            Buffer(Renderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memPropertyFlags);
            Buffer(Buffer &&other) = default;
            Buffer(const Buffer &other) = delete;
            ~Buffer() {}

            auto operator=(const Buffer &other) -> Buffer& = delete;
            auto operator=(Buffer &&other) -> Buffer& = default;

            operator VkBuffer() { return buffer; }

            auto getHandle() const -> VkBuffer { return buffer; }
            auto getSize() const -> uint32_t { return size; }

            void updateAll(const void *newData) const;
            void updatePart(const void *newData, uint32_t offset, uint32_t size);
            void transferTo(const Buffer& other, VkQueue queue, VkCommandPool cmdPool) const;

        private:
            VkDevice device = nullptr;
            Renderer *renderer = nullptr;
            Resource<VkDeviceMemory> memory;
            Resource<VkBuffer> buffer;
            VkDeviceSize size = 0;
        };
    }
}

#endif