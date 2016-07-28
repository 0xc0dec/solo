#include "SoloVulkanRenderer.h"
#include "SoloSDLVulkanDevice.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;


VkFormat getDepthFormat(VkPhysicalDevice device)
{
	// Since all depth formats may be optional, we need to find a suitable depth format to use
	// Start with the highest precision packed format
	std::vector<VkFormat> depthFormats =
    {
		VK_FORMAT_D32_SFLOAT_S8_UINT, 
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT, 
		VK_FORMAT_D16_UNORM_S8_UINT, 
		VK_FORMAT_D16_UNORM 
	};

	for (auto& format : depthFormats)
	{
		VkFormatProperties formatProps;
		vkGetPhysicalDeviceFormatProperties(device, format, &formatProps);
		// Format must support depth stencil attachment for optimal tiling
		if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            return format;
	}

    SL_EXCEPTION(InternalException, "Could not detect suitable depth format");
}


std::tuple<VkFormat, VkColorSpaceKHR> getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t count;
    SL_CHECK_VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr), "Failed to get surface format count");

    std::vector<VkSurfaceFormatKHR> formats(count);
    SL_CHECK_VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data()), "Failed to get surface formats");

    if (count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
        return std::make_tuple(VK_FORMAT_B8G8R8A8_UNORM, formats[0].colorSpace);
    return std::make_tuple(formats[0].format, formats[0].colorSpace);
}


uint32_t getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    SL_EXCEPTION_IF(count <= 0, InternalException, "Failed to get count of graphics queues");

    std::vector<VkQueueFamilyProperties> queueProps;
	queueProps.resize(count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueProps.data());

    std::vector<VkBool32> presentSupported(count);
    for (uint32_t i = 0; i < count; i++)
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupported[i]);

    // TODO support for separate rendering and presenting queues
    for (uint32_t i = 0; i < count; i++)
	{
        if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupported[i] == VK_TRUE)
            return i;
	}

    SL_EXCEPTION(InternalException, "Failed to detect queue index");
}


void VulkanRenderer::createDevice(uint32_t queueIndex)
{
    std::vector<float> queuePriorities = { 0.0f };
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = {};
	queueCreateInfos.resize(1);
	queueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfos[0].queueFamilyIndex = queueIndex;
	queueCreateInfos[0].queueCount = 1;
	queueCreateInfos[0].pQueuePriorities = queuePriorities.data();

    std::vector<const char*> deviceExtensions;
    deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    VkDeviceCreateInfo deviceCreateInfo = {};
    std::vector<VkPhysicalDeviceFeatures> enabledFeatures = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = enabledFeatures.data();
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    SL_CHECK_VK_CALL(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device), "Failed to create logical device");
}


void VulkanRenderer::detectPhysicalDevice(VkInstance instance)
{
    uint32_t gpuCount = 0;
    SL_CHECK_VK_CALL(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr), "Failed to get GPU count");
    SL_EXCEPTION_IF(gpuCount == 0, InternalException, "No GPU found");
	
	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	SL_CHECK_VK_CALL(vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data()), "Failed to enumerate devices");

    physicalDevice = physicalDevices[0]; // TODO at least for now
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);
}


void VulkanRenderer::createSemaphores()
{
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.flags = 0;

    SL_CHECK_VK_CALL(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &presentCompleteSem), "Failed to create present semaphore");
    SL_CHECK_VK_CALL(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderCompleteSem), "Failed to create render semaphore");
}


VulkanRenderer::VulkanRenderer(Device* engineDevice)
{
    auto vulkanDevice = dynamic_cast<SDLVulkanDevice*>(engineDevice);
    auto instance = vulkanDevice->getVulkanInstance();
    auto surface = vulkanDevice->getVulkanSurface();

    detectPhysicalDevice(instance);

    auto surfaceFormats = getSurfaceFormats(physicalDevice, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);

    auto queueIndex = getQueueIndex(physicalDevice, surface);

    createDevice(queueIndex);

    vkGetDeviceQueue(device, queueIndex, 0, &queue);

    depthFormat = getDepthFormat(physicalDevice);

    createSemaphores();

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &presentCompleteSem;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderCompleteSem;

    VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueIndex;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    SL_CHECK_VK_CALL(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool), "Failed to create command pool");
}


VulkanRenderer::~VulkanRenderer()
{
    // TODO
}


#else
#   error Vulkan renderer is not supported on this platform
#endif