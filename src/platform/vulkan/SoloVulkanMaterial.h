// Note: this file is a total hackery and a work-in-progress, so avoid reading this

#pragma once

#include "SoloCommon.h"
#include "SoloVulkan.h"


namespace solo
{
    class VulkanMaterial
    {
    public:
        SL_NONCOPYABLE(VulkanMaterial)
        VulkanMaterial(VkDevice device);

        void rebuild();

    private:
        static VkDescriptorPool descPool;

        VkDevice device = nullptr;
    };
}