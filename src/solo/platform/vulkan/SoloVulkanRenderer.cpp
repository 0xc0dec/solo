#include "SoloVulkanRenderer.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;


void VulkanRenderer::beginFrame()
{
}


void VulkanRenderer::endFrame()
{
}


#else
#   error Vulkan renderer is not supported on this platform
#endif
