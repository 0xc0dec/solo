#include "SoloSDLVulkanDevice.h"

using namespace solo;


SDLVulkanDevice::SDLVulkanDevice(const DeviceCreationArgs& args):
    SDLDevice(args)
{
    VkApplicationInfo appInfo = {};
    
    VkInstanceCreateInfo instanceCreateInfo = {};
    vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
}


void SDLVulkanDevice::saveScreenshot(const std::string& path)
{
}


auto SDLVulkanDevice::getCanvasSize() const -> Vector2
{
    return Vector2(); // TODO
}


void SDLVulkanDevice::endUpdate()
{

}
