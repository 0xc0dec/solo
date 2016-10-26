#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#ifdef SL_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR 
#endif
#include <vulkan.h>

#ifdef SL_ERR_CHECK
#   define SL_CHECK_VK_RESULT(vkCall, ...) SL_BLOCK(if (vkCall != VK_SUCCESS) SL_ERR(__VA_ARGS__))
#else
#   define SL_CHECK_VK_RESULT(vkCall) vkCall
#endif

namespace solo
{
    
}

#else
#   error Vulkan renderer is not supported on this platform
#endif
