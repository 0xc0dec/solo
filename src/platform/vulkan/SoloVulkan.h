#pragma once

#include "SoloCommon.h"

#ifdef SL_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR 
#endif
#include <vulkan.h>

#define SL_CHECK_VK_CALL(call, message) \
    do { \
        if (call != VK_SUCCESS) \
            SL_EXCEPTION(InternalException, message); \
    } while (0)