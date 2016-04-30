#include "SoloSDLVulkanDevice.h"
#include <array>

using namespace solo;


const char* Name = "SoloVulkanDevice";


SDLVulkanDevice::SDLVulkanDevice(const DeviceCreationArgs& args):
    SDLDevice(args)
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = Name;
    appInfo.pEngineName = Name;
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Only Windows so far
    std::array<const char*, 2> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

    if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance))
        SL_THROW(InternalException, "Failed to initialize Vulkan instance");

    uint32_t gpuCount = 0;
    if (vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr))
        SL_THROW(InternalException, "Failed to get physical devices count");

    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    if (vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data()))
        SL_THROW(InternalException, "Failed to enumerate physical devices");

    physicalDevice = physicalDevices[0];

    uint32_t queueIndex = 0;
    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueProps;
    queueProps.resize(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueProps.data());
    for (queueIndex = 0; queueIndex < queueCount; queueIndex++)
    {
        if (queueProps[queueIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            break;
    }

    std::array<float, 1> queuePriorities = { 0.0f };
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = queuePriorities.data();

    std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.pEnabledFeatures = nullptr;

    if (deviceExtensions.size() > 0)
    {
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    }

    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device))
        SL_THROW(InternalException, "Failed to create Vulkan device");

    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);
    vkGetDeviceQueue(device, queueIndex, 0, &queue);

    if (!selectDepthFormat())
        SL_THROW(InternalException, "Failed to initialize depth format");

//    swapChain.connect(instance, physicalDevice, device);

    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = nullptr;
    semaphoreCreateInfo.flags = 0;

    if (vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &presentCompleteSem) ||
        vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderCompleteSem))
        SL_THROW(InternalException, "Failed to initialize semaphores");

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &presentCompleteSem;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderCompleteSem;
}


bool SDLVulkanDevice::selectDepthFormat()
{
    std::vector<VkFormat> formats =
    {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM
    };

    for (auto& format : formats)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
        // Format must support depth stencil attachment for optimal tiling
        if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            depthFormat = format;
            return true;
        }
    }

    return false;
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
