#include "SoloVulkanRenderer.h"
#include "SoloSDLVulkanDevice.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;


VulkanRenderer::VulkanRenderer(Device* device)
{
    auto vulkanDevice = dynamic_cast<SDLVulkanDevice*>(device);
}


#else
#   error Vulkan renderer is not supported on this platform
#endif