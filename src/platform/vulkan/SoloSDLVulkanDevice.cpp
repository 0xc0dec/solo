#include "SoloSDLVulkanDevice.h"

using namespace solo;


VulkanDevice::VulkanDevice(const DeviceCreationArgs& args):
    Device(args)
{
    VkApplicationInfo appInfo = {};
    
    VkInstanceCreateInfo instanceCreateInfo = {};
    vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
}
