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
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);

    std::vector<VkSurfaceFormatKHR> formats(count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data());

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

    vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);
}


void VulkanRenderer::initPhysicalDevice(VkInstance instance)
{
    uint32_t gpuCount = 0;
    vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
	
	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data());

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

    vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &presentCompleteSem);
    vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &renderCompleteSem);
}


void VulkanRenderer::initCommandPool(uint32_t queueIndex)
{
    VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueIndex;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool);
}


void VulkanRenderer::initSwapchain(VkSurfaceKHR surface, bool vsync)
{
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

    std::vector<VkPresentModeKHR> presentModes;
    presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

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
    vkCreateSwapchainKHR(logicalDevice, &swapchainInfo, nullptr, &swapchain);
    
    uint32_t imageCount = 0;
    vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, nullptr);

    std::vector<VkImage> images;
    images.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, images.data());

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

        vkCreateImageView(logicalDevice, &imageInfo, nullptr, &swapchainBuffers[i].imageView);
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

    vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, drawCmdBuffers.data());
    vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, prePresentCmdBuffers.data());
    vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, postPresentCmdBuffers.data());

    VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr;

    for (uint32_t i = 0; i < count; i++)
    {
        // Transform the image back to a color attachment that our render pass can write to

        vkBeginCommandBuffer(postPresentCmdBuffers[i], &beginInfo);

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
        vkEndCommandBuffer(postPresentCmdBuffers[i]);

        // Transforms the (framebuffer) image layout from color attachment to present(khr) for presenting to the swap chain

        vkBeginCommandBuffer(prePresentCmdBuffers[i], &beginInfo);
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

        vkEndCommandBuffer(prePresentCmdBuffers[i]);
    }
}


void VulkanRenderer::initDepthStencil()
{
    VkImageCreateInfo image = {};
	image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image.pNext = nullptr;
	image.imageType = VK_IMAGE_TYPE_2D;
	image.format = depthFormat;
	image.extent = { canvasWidth, canvasHeight, 1 };
	image.mipLevels = 1;
	image.arrayLayers = 1;
	image.samples = VK_SAMPLE_COUNT_1_BIT;
	image.tiling = VK_IMAGE_TILING_OPTIMAL;
	image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	image.flags = 0;

    VkMemoryAllocateInfo alloc = {};
	alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc.pNext = nullptr;
	alloc.allocationSize = 0;
	alloc.memoryTypeIndex = 0;

    VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = depthFormat;
	createInfo.flags = 0;
	createInfo.subresourceRange = {};
	createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

    VkMemoryRequirements memReqs;
	vkCreateImage(logicalDevice, &image, nullptr, &depthStencil.image);
	vkGetImageMemoryRequirements(logicalDevice, depthStencil.image, &memReqs);

    auto memTypeIndex = findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    SL_EXCEPTION_IF(memTypeIndex < 0, InternalException);

	alloc.allocationSize = memReqs.size;
    alloc.memoryTypeIndex = memTypeIndex;
	vkAllocateMemory(logicalDevice, &alloc, nullptr, &depthStencil.mem);

    vkBindImageMemory(logicalDevice, depthStencil.image, depthStencil.mem, 0);
    
    setImageLayout(setupCmdBuffer, depthStencil.image, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    createInfo.image = depthStencil.image;
    vkCreateImageView(logicalDevice, &createInfo, nullptr, &depthStencil.view);
}


void VulkanRenderer::initRenderPass()
{
    VkAttachmentDescription attachments[2] = {};

    // Color attachment
	attachments[0].format = colorFormat;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// Depth attachment
	attachments[1].format = depthFormat;
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorReference = {};
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference = {};
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.flags = 0;
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = nullptr;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorReference;
	subpass.pResolveAttachments = nullptr;
	subpass.pDepthStencilAttachment = &depthReference;
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments = nullptr;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.pNext = nullptr;
	renderPassInfo.attachmentCount = 2;
	renderPassInfo.pAttachments = attachments;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 0;
	renderPassInfo.pDependencies = nullptr;

    vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass);
}


int32_t VulkanRenderer::findMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties)
{
    for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((typeBits & 1) == 1)
		{
			if ((physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}
		typeBits >>= 1;
	}
    return -1;
}


VkCommandBuffer VulkanRenderer::createCommandBuffer()
{
    VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandPool = commandPool;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandBufferCount = 1;

    VkCommandBuffer result = nullptr;
    vkAllocateCommandBuffers(logicalDevice, &allocateInfo, &result);

    return result;
}


void VulkanRenderer::destroyCommandBuffer(VkCommandBuffer buffer)
{
    vkFreeCommandBuffers(logicalDevice, commandPool, 1, &buffer);
}


void VulkanRenderer::beginCommandBuffer(VkCommandBuffer buffer)
{
    VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    vkBeginCommandBuffer(buffer, &beginInfo);
}


void VulkanRenderer::flushCommandBuffer(VkCommandBuffer buffer)
{
    vkEndCommandBuffer(buffer);

    VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &buffer;

    vkQueueSubmit(queue, 1, &submitInfo, nullptr);
	vkQueueWaitIdle(queue);
}


void VulkanRenderer::setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask,
    VkImageLayout oldLayout, VkImageLayout newLayout, VkImageSubresourceRange subresourceRange)
{
    VkImageMemoryBarrier imageMemoryBarrier = {};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.pNext = nullptr;
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.oldLayout = oldLayout;
	imageMemoryBarrier.newLayout = newLayout;
	imageMemoryBarrier.image = image;
	imageMemoryBarrier.subresourceRange = subresourceRange;

    switch (oldLayout)
	{
	    case VK_IMAGE_LAYOUT_UNDEFINED:
			    imageMemoryBarrier.srcAccessMask = 0;
			    break;
	    case VK_IMAGE_LAYOUT_PREINITIALIZED:
			    imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
			    break;
	    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			    imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			    break;
	    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			    imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			    break;
	    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			    break;
	    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			    break;
	    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			    imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			    break;
        default:
            SL_ASSERT(false);
	}

    switch (newLayout)
	{
	    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
		    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		    break;
	    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
		    imageMemoryBarrier.srcAccessMask = imageMemoryBarrier.srcAccessMask | VK_ACCESS_TRANSFER_READ_BIT;
		    imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		    break;
	    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
		    imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		    imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		    break;
	    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
		    imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		    break;
	    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
		    if (imageMemoryBarrier.srcAccessMask == 0)
			    imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
		    imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		    break;
        default:
            SL_ASSERT(false);
	}

    VkPipelineStageFlags srcStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	VkPipelineStageFlags destStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    vkCmdPipelineBarrier(cmdbuffer, srcStageFlags, destStageFlags, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
}


void VulkanRenderer::setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkImageSubresourceRange subresourceRange = {};
	subresourceRange.aspectMask = aspectMask;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = 1;
	subresourceRange.layerCount = 1;
    setImageLayout(cmdbuffer, image, aspectMask, oldLayout, newLayout, subresourceRange);
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

    setupCmdBuffer = createCommandBuffer();
    beginCommandBuffer(setupCmdBuffer);

    initDepthStencil();
    initRenderPass();

    flushCommandBuffer(setupCmdBuffer);
    destroyCommandBuffer(setupCmdBuffer);
}


VulkanRenderer::~VulkanRenderer()
{
    // TODO
}


#else
#   error Vulkan renderer is not supported on this platform
#endif