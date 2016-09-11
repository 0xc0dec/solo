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

#pragma once

#include "SoloCommon.h"
#include "SoloVulkan.h"

namespace solo
{
    class VulkanBuffer
    {
    public:
        VulkanBuffer() {}
        VulkanBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
            VkPhysicalDeviceMemoryProperties memProps);
        VulkanBuffer(VulkanBuffer&& other);
        VulkanBuffer(const VulkanBuffer& other) = delete;

        ~VulkanBuffer();

        VulkanBuffer &operator=(const VulkanBuffer& other) = delete;
        VulkanBuffer &operator=(VulkanBuffer&& other);

        VkBuffer& getHandle();

        void update(void* newData);
        void transferTo(const VulkanBuffer& other, VkQueue queue, VkCommandPool cmdPool);

    private:
        void cleanup();

        VkDevice device = nullptr;
        VkBuffer buffer = nullptr;
        VkDeviceMemory memory = nullptr;
        VkDeviceSize size = 0;
    };

    inline VkBuffer& VulkanBuffer::getHandle()
    {
        return buffer;
    }
}