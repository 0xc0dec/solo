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
    SL_CHECK_VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr));

    std::vector<VkSurfaceFormatKHR> formats(count);
    SL_CHECK_VK_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data()));

    if (count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
        return std::make_tuple(VK_FORMAT_B8G8R8A8_UNORM, formats[0].colorSpace);
    return std::make_tuple(formats[0].format, formats[0].colorSpace);
}


uint32_t getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    SL_EXCEPTION_IF(count <= 0, InternalException);

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

    SL_EXCEPTION(InternalException);
}


void VulkanRenderer::initLogicalDevice(uint32_t queueIndex)
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

    SL_CHECK_VK_CALL(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice));
}


void VulkanRenderer::initPhysicalDevice(VkInstance instance)
{
    uint32_t gpuCount = 0;
    SL_CHECK_VK_CALL(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));
    SL_EXCEPTION_IF(gpuCount == 0, InternalException, "No GPU found");
	
	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	SL_CHECK_VK_CALL(vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data()));

    physicalDevice = physicalDevices[0]; // TODO at least for now
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);
}


void VulkanRenderer::initSemaphores()
{
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.flags = 0;

    SL_CHECK_VK_CALL(vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &presentCompleteSem));
    SL_CHECK_VK_CALL(vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &renderCompleteSem));
}


void VulkanRenderer::initCommandPool(uint32_t queueIndex)
{
    VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueIndex;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    SL_CHECK_VK_CALL(vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool));
}


void VulkanRenderer::initSwapchain(VkSurfaceKHR surface, bool vsync)
{
	VkSurfaceCapabilitiesKHR capabilities;
	SL_CHECK_VK_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));

    uint32_t presentModeCount;
    SL_CHECK_VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr));
    SL_EXCEPTION_IF(presentModeCount == 0, InternalException, "No surface present modes detected");

    std::vector<VkPresentModeKHR> presentModes;
    presentModes.resize(presentModeCount);
    SL_CHECK_VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data()));

    if (capabilities.currentExtent.width == -1)
    {
        // Surface extent not defined - select based on device canvas size
        auto deviceCanvasSize = device->getCanvasSize();
        canvasWidth = deviceCanvasSize.x;
        canvasHeight = deviceCanvasSize.y;
    }
    else
    {
        canvasWidth = capabilities.currentExtent.width;
        canvasHeight = capabilities.currentExtent.height;
    }

    // Select present mode
    auto presentMode = VK_PRESENT_MODE_FIFO_KHR; // "vsync"

    if (!vsync)
    {
        for (const auto mode: presentModes)
        {
            if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
                presentMode = mode;
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                presentMode = mode;
                break;
            }
        }
    }

    VkSurfaceTransformFlagsKHR transformFlags;
    if (capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        transformFlags = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    else
        transformFlags = capabilities.currentTransform;

    auto requestedImageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && requestedImageCount > capabilities.maxImageCount)
        requestedImageCount = capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR swapchainInfo = {};
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.pNext = nullptr;
	swapchainInfo.surface = surface;
	swapchainInfo.minImageCount = requestedImageCount;
	swapchainInfo.imageFormat = colorFormat;
	swapchainInfo.imageColorSpace = colorSpace;
	swapchainInfo.imageExtent = { canvasWidth, canvasHeight };
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainInfo.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(transformFlags);
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainInfo.queueFamilyIndexCount = 0;
	swapchainInfo.pQueueFamilyIndices = nullptr;
	swapchainInfo.presentMode = presentMode;
	swapchainInfo.oldSwapchain = nullptr; // TODO
	swapchainInfo.clipped = VK_TRUE;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    SL_CHECK_VK_CALL(vkCreateSwapchainKHR(logicalDevice, &swapchainInfo, nullptr, &swapchain));
    
    uint32_t imageCount = 0;
    SL_CHECK_VK_CALL(vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, nullptr));

    std::vector<VkImage> images;
    images.resize(imageCount);
    SL_CHECK_VK_CALL(vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, images.data()));

    swapchainBuffers.resize(imageCount);
    for (uint32_t i = 0; i < imageCount; i++)
    {
        VkImageViewCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageInfo.pNext = nullptr;
		imageInfo.format = colorFormat;
		imageInfo.components = {
			VK_COMPONENT_SWIZZLE_R,
			VK_COMPONENT_SWIZZLE_G,
			VK_COMPONENT_SWIZZLE_B,
			VK_COMPONENT_SWIZZLE_A
		};
		imageInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageInfo.subresourceRange.baseMipLevel = 0;
		imageInfo.subresourceRange.levelCount = 1;
		imageInfo.subresourceRange.baseArrayLayer = 0;
		imageInfo.subresourceRange.layerCount = 1;
		imageInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageInfo.flags = 0;
        imageInfo.image = images[i];

        swapchainBuffers[i].image = images[i];

        SL_CHECK_VK_CALL(vkCreateImageView(logicalDevice, &imageInfo, nullptr, &swapchainBuffers[i].imageView));
    }
}


void VulkanRenderer::initCommandBuffers()
{
    auto count = swapchainBuffers.size();

    drawCmdBuffers.resize(count);
    prePresentCmdBuffers.resize(count);
    postPresentCmdBuffers.resize(count);

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = count;

    SL_CHECK_VK_CALL(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, drawCmdBuffers.data()));
    SL_CHECK_VK_CALL(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, prePresentCmdBuffers.data()));
    SL_CHECK_VK_CALL(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, postPresentCmdBuffers.data()));

    VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr;

    for (uint32_t i = 0; i < count; i++)
    {
        // Transform the image back to a color attachment that our render pass can write to

        SL_CHECK_VK_CALL(vkBeginCommandBuffer(postPresentCmdBuffers[i], &beginInfo));

        VkImageMemoryBarrier postPresentBarrier = {};
	    postPresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	    postPresentBarrier.pNext = nullptr;
	    postPresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	    postPresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        postPresentBarrier.srcAccessMask = 0;
		postPresentBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		postPresentBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		postPresentBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		postPresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		postPresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		postPresentBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		postPresentBarrier.image = swapchainBuffers[i].image;

        vkCmdPipelineBarrier(postPresentCmdBuffers[i], VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			0, 0, nullptr, 0, nullptr, 1, &postPresentBarrier);
        SL_CHECK_VK_CALL(vkEndCommandBuffer(postPresentCmdBuffers[i]));

        // Transforms the (framebuffer) image layout from color attachment to present(khr) for presenting to the swap chain

        SL_CHECK_VK_CALL(vkBeginCommandBuffer(prePresentCmdBuffers[i], &beginInfo));
        VkImageMemoryBarrier prePresentBarrier = {};
	    prePresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	    prePresentBarrier.pNext = nullptr;
	    prePresentBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		prePresentBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		prePresentBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		prePresentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		prePresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		prePresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		prePresentBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		prePresentBarrier.image = swapchainBuffers[i].image;

        vkCmdPipelineBarrier(prePresentCmdBuffers[i], VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			0, 0, nullptr, 0, nullptr, 1, &prePresentBarrier);

        SL_CHECK_VK_CALL(vkEndCommandBuffer(prePresentCmdBuffers[i]));
    }
}


VulkanRenderer::VulkanRenderer(Device* engineDevice):
    device(dynamic_cast<SDLVulkanDevice*>(engineDevice))
{
    auto instance = device->getVulkanInstance();
    auto surface = device->getVulkanSurface();

    initPhysicalDevice(instance);

    auto surfaceFormats = getSurfaceFormats(physicalDevice, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);

    auto queueIndex = getQueueIndex(physicalDevice, surface);

    initLogicalDevice(queueIndex);

    vkGetDeviceQueue(logicalDevice, queueIndex, 0, &queue);

    depthFormat = getDepthFormat(physicalDevice);

    initSemaphores();

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &presentCompleteSem;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderCompleteSem;

    initCommandPool(queueIndex);
    initSwapchain(surface, device->getSetup().vsync);
    initCommandBuffers();
}


VulkanRenderer::~VulkanRenderer()
{
    // TODO
}


#else
#   error Vulkan renderer is not supported on this platform
#endif