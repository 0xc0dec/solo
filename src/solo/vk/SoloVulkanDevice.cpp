/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanDevice.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackFunc(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
    u64 obj, size_t location, s32 code, const s8 *layerPrefix, const s8 *msg, void *userData)
{
    SL_DEBUG_LOG("Vulkan: ", msg);
    return VK_FALSE;
}

static auto createDebugCallback(VkInstance instance, PFN_vkDebugReportCallbackEXT callbackFunc) -> VulkanResource<VkDebugReportCallbackEXT>
{
    VkDebugReportCallbackCreateInfoEXT createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = callbackFunc;

    const auto create = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
    asrt(create, "Unable to load pointer to vkCreateDebugReportCallbackEXT");

    const auto destroy = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
    asrt(destroy, "Unable to load pointer to vkDestroyDebugReportCallbackEXT");

    VulkanResource<VkDebugReportCallbackEXT> result{instance, destroy};
    SL_VK_CHECK_RESULT(create(instance, &createInfo, nullptr, result.cleanRef()));

    return result;
}

static auto selectSurfaceFormat(VkPhysicalDevice device, VkSurfaceKHR surface) -> std::tuple<VkFormat, VkColorSpaceKHR>
{
    u32 count;
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr));

    vec<VkSurfaceFormatKHR> formats(count);
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data()));

    if (count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
        return {VK_FORMAT_B8G8R8A8_UNORM, formats[0].colorSpace};
    return {formats[0].format, formats[0].colorSpace};
}

static auto selectQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface) -> u32
{
    u32 count;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

    vec<VkQueueFamilyProperties> queueProps;
    queueProps.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueProps.data());

    vec<VkBool32> presentSupported(count);
    for (u32 i = 0; i < count; i++)
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupported[i]));

    // TODO support for separate rendering and presenting queues
    for (u32 i = 0; i < count; i++)
    {
        if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupported[i] == VK_TRUE)
            return i;
    }

    asrt(false, "Unable to find queue index");
	
    return 0;
}

static auto createDevice(VkPhysicalDevice physicalDevice, u32 queueIndex) -> VulkanResource<VkDevice>
{
    vec<float> queuePriorities = {0.0f};
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = queuePriorities.data();

    vec<const s8*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkPhysicalDeviceFeatures enabledFeatures{};
    enabledFeatures.samplerAnisotropy = true;

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
    deviceCreateInfo.enabledExtensionCount = static_cast<u32>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    VulkanResource<VkDevice> result{vkDestroyDevice};
    SL_VK_CHECK_RESULT(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, result.cleanRef()));

    return result;
}

static auto createCommandPool(VkDevice device, u32 queueIndex) -> VulkanResource<VkCommandPool>
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VulkanResource<VkCommandPool> commandPool{device, vkDestroyCommandPool};
    SL_VK_CHECK_RESULT(vkCreateCommandPool(device, &poolInfo, nullptr, commandPool.cleanRef()));

    return commandPool;
}

VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface):
    surface_(surface)
{
#ifdef SL_DEBUG
    debugCallback_ = createDebugCallback(instance, debugCallbackFunc);
#endif

    selectPhysicalDevice(instance);

    detectFormatSupport(VK_FORMAT_R8_UNORM);
    detectFormatSupport(VK_FORMAT_R8G8B8A8_UNORM);
    detectFormatSupport(VK_FORMAT_R16G16B16A16_SFLOAT);
    detectFormatSupport(VK_FORMAT_D32_SFLOAT);
    detectFormatSupport(VK_FORMAT_D32_SFLOAT_S8_UINT);
    detectFormatSupport(VK_FORMAT_D24_UNORM_S8_UINT);
    detectFormatSupport(VK_FORMAT_D16_UNORM_S8_UINT);
    detectFormatSupport(VK_FORMAT_D16_UNORM);

    auto surfaceFormats = ::selectSurfaceFormat(physical_, surface);
    colorFormat_ = std::get<0>(surfaceFormats);
    colorSpace_ = std::get<1>(surfaceFormats);
    depthFormat_ = selectDepthFormat();

    const auto queueIndex = selectQueueIndex(physical_, surface);
    handle_ = createDevice(physical_, queueIndex);
    vkGetDeviceQueue(handle_, queueIndex, 0, &queue_);

    commandPool_ = createCommandPool(handle_, queueIndex);
}

bool VulkanDevice::isFormatSupported(VkFormat format, VkFormatFeatureFlags features) const
{
    return supportedFormats_.count(format) && (supportedFormats_.at(format) & features) == features;
}

void VulkanDevice::detectFormatSupport(VkFormat format)
{
    // TODO Check for linear tiling as well
    VkFormatProperties formatProps;
    vkGetPhysicalDeviceFormatProperties(physical_, format, &formatProps);
    if (formatProps.optimalTilingFeatures)
        supportedFormats_[format] = formatProps.optimalTilingFeatures;
}

auto VulkanDevice::selectDepthFormat() const -> VkFormat
{
    vec<VkFormat> depthFormats =
    {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM
    };

    for (auto &format : depthFormats)
    {
        if (isFormatSupported(format, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT))
            return format;
    }

    asrt(false, "Unable to pick depth format");
	
    return VK_FORMAT_UNDEFINED;
}

void VulkanDevice::selectPhysicalDevice(VkInstance instance)
{
    u32 gpuCount = 0;
    SL_VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));

    vec<VkPhysicalDevice> devices(gpuCount);
    SL_VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, devices.data()));

    for (const auto &device: devices)
    {
        vkGetPhysicalDeviceProperties(device, &physicalProperties_);
        vkGetPhysicalDeviceFeatures(device, &physicalFeatures_);
        vkGetPhysicalDeviceMemoryProperties(device, &physicalMemoryFeatures_);

        physical_ = device;

        // Select discrete GPU if present, otherwise any other
        if (physicalProperties_.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            return;
    }
}

#endif
