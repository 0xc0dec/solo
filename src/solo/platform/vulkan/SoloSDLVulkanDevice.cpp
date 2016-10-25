#include "SoloSDLVulkanDevice.h"

using namespace solo;


SDLVulkanDevice::SDLVulkanDevice(const DeviceSetup& setup):
    SDLDevice(setup)
{
}


auto SDLVulkanDevice::getCanvasSize() const -> Vector2
{
    return Vector2();
}


void SDLVulkanDevice::saveScreenshot(const std::string& path)
{
}


void SDLVulkanDevice::endUpdate()
{
}
