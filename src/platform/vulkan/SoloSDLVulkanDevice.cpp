#include "SoloSDLVulkanDevice.h"

#ifdef SL_OPENGL_RENDERER

using namespace solo;


#else
#   error Vulkan renderer is not supported on this platform
#endif