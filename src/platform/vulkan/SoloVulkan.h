#pragma once

#include "SoloCommon.h"

#ifdef SL_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR 
#endif
#include <vulkan.h>

#ifdef SL_ERR_CHECK
#   define SL_CHECK_VK_RESULT(result, ...) SL_DBG_BLOCK(if (result != VK_SUCCESS) SL_ERR(__VA_ARGS__))
#else
#   define SL_CHECK_VK_RESULT(result)
#endif