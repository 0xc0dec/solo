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
        // TODO currently supports pools that allocate only one predefined type of sets
        class DescriptorPool
        {
        public:
            DescriptorPool() {}
            DescriptorPool(VkDevice device, VkDescriptorType type, uint32_t descriptorCount, uint32_t maxSetCount);
            DescriptorPool(const DescriptorPool &other) = delete;
            DescriptorPool(DescriptorPool &&other) noexcept;

            ~DescriptorPool() {}

            auto operator=(DescriptorPool other) noexcept -> DescriptorPool&;

            auto allocateSet(VkDescriptorSetLayout layout) const->VkDescriptorSet;

        private:
            VkDevice device = nullptr;
            Resource<VkDescriptorPool> pool;

            void swap(DescriptorPool &other) noexcept;
        };
    }
}

#endif