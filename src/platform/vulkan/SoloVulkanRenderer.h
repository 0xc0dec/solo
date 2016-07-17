#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

namespace solo
{
    class VulkanRenderer
    {
    };
}

#else
#   error Vulkan renderer is not supported on this platform
#endif