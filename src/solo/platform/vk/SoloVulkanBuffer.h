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
        class Buffer
        {
        public:
            enum Flags
            {
                Host = 1,
                Device = 1 << 1,
                Uniform = 1 << 2,
                Vertex = 1 << 3,
                TransferSrc = 1 << 4,
                TransferDst = 1 << 5
            };

            Buffer() {}
            Buffer(VkDevice device, VkDeviceSize size, uint32_t flags, VkPhysicalDeviceMemoryProperties memProps);
            Buffer(Buffer &&other) noexcept;
            Buffer(const Buffer &other) = delete;

            ~Buffer() {}

            auto operator=(Buffer other) noexcept -> Buffer&;

            auto getHandle() -> VkBuffer;

            void update(const void *newData) const;
            void transferTo(const Buffer& other, VkQueue queue, VkCommandPool cmdPool) const;

        private:
            VkDevice device = nullptr;
            Resource<VkDeviceMemory> memory;
            Resource<VkBuffer> buffer;
            VkDeviceSize size = 0;

            void swap(Buffer &other) noexcept;
        };

        inline auto Buffer::getHandle() -> VkBuffer
        {
            return buffer;
        }
    }
}

#endif