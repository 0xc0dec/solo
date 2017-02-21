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

            auto operator=(Buffer other) noexcept->Buffer&;

            auto getHandle()->VkBuffer;

            void update(void *newData) const;
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