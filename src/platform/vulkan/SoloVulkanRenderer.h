#pragma once

#include "SoloCommon.h"
#include <vulkan.h>

#ifdef SL_VULKAN_RENDERER

namespace solo
{
    class VulkanRenderer final
    {
    public:
        
    private:
    };
}

#else
#   error Vulkan renderer is not supported on this platform
#endif