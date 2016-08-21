#include "SoloVulkanRenderer.h"
#include "SoloSDLVulkanDevice.h"
#include "SoloFileSystem.h" // TODO remove?

#ifdef SL_VULKAN_RENDERER

using namespace solo;

// TODO perhaps rename all "Framebuffer" to "FrameBuffer"


VkShaderModule createShader(VkDevice logicalDevice, const std::vector<uint8_t>& data)
{
    VkShaderModuleCreateInfo shaderModuleInfo {};
    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.pNext = nullptr;
    shaderModuleInfo.flags = 0;
    shaderModuleInfo.codeSize = data.size();
    shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(data.data());

    VkShaderModule module = nullptr;
    SL_CHECK_VK_RESULT(vkCreateShaderModule(logicalDevice, &shaderModuleInfo, nullptr, &module));

    return module;
}


void beginCommandBuffer(VkCommandBuffer buffer)
{
    VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    SL_CHECK_VK_RESULT(vkBeginCommandBuffer(buffer, &beginInfo));
}


void destroyCommandBuffers(VkDevice logicalDevice, VkCommandPool commandPool, VkCommandBuffer* buffers, uint32_t count)
{
    vkFreeCommandBuffers(logicalDevice, commandPool, count, buffers);
}


void flushCommandBuffer(VkQueue queue, VkCommandBuffer buffer)
{
    SL_CHECK_VK_RESULT(vkEndCommandBuffer(buffer));

    VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &buffer;

    SL_CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, nullptr));
	SL_CHECK_VK_RESULT(vkQueueWaitIdle(queue));
}


int32_t findMemoryType(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties, uint32_t typeBits, VkMemoryPropertyFlags properties)
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

    return VK_FORMAT_UNDEFINED;
}


std::tuple<VkFormat, VkColorSpaceKHR> getSurfaceFormats(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t count;
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr));

    std::vector<VkSurfaceFormatKHR> formats(count);
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data()));

    if (count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
        return std::make_tuple(VK_FORMAT_B8G8R8A8_UNORM, formats[0].colorSpace);
    return std::make_tuple(formats[0].format, formats[0].colorSpace);
}


uint32_t getQueueIndex(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

    std::vector<VkQueueFamilyProperties> queueProps;
	queueProps.resize(count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueProps.data());

    std::vector<VkBool32> presentSupported(count);
    for (uint32_t i = 0; i < count; i++)
        SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupported[i]));

    // TODO support for separate rendering and presenting queues
    for (uint32_t i = 0; i < count; i++)
	{
        if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupported[i] == VK_TRUE)
            return i;
	}

    SL_ERR("Could not find queue index");
    return 0;
}


VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice, uint32_t queueIndex)
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

    VkDevice logicalDevice = nullptr;
    SL_CHECK_VK_RESULT(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice));

    return logicalDevice;
}


VkPhysicalDevice createPhysicalDevice(VkInstance instance)
{
    uint32_t gpuCount = 0;
    SL_CHECK_VK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));
	
	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	SL_CHECK_VK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data()));

    return physicalDevices[0]; // TODO at least for now
}


VkSemaphore createSemaphore(VkDevice logicalDevice)
{
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.flags = 0;

    VkSemaphore semaphore = nullptr;
    SL_CHECK_VK_RESULT(vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &semaphore));

    return semaphore;
}


VkCommandPool createCommandPool(VkDevice logicalDevice, uint32_t queueIndex)
{
    VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueIndex;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VkCommandPool commandPool = nullptr;
    SL_CHECK_VK_RESULT(vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool));

    return commandPool;
}


void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask,
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
            SL_ERR("Unknown old image layout");
            break;
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
            SL_ERR("Unknown new image layout");
            break;
	}

    VkPipelineStageFlags srcStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	VkPipelineStageFlags destStageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    vkCmdPipelineBarrier(cmdbuffer, srcStageFlags, destStageFlags, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
}


void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkImageSubresourceRange subresourceRange = {};
	subresourceRange.aspectMask = aspectMask;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = 1;
	subresourceRange.layerCount = 1;
    setImageLayout(cmdbuffer, image, aspectMask, oldLayout, newLayout, subresourceRange);
}


VkRenderPass createRenderPass(VkDevice logicalDevice, VkFormat colorFormat, VkFormat depthFormat)
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

    VkRenderPass renderPass = nullptr;
    SL_CHECK_VK_RESULT(vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass));

    return renderPass;
}


VkCommandBuffer createCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool)
{
    VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandPool = commandPool;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandBufferCount = 1;

    VkCommandBuffer result = nullptr;
    SL_CHECK_VK_RESULT(vkAllocateCommandBuffers(logicalDevice, &allocateInfo, &result));

    return result;
}


VkFramebuffer createFrameBuffer(VkDevice logicalDevice, VkImageView colorAttachment, VkImageView depthAttachment,
    VkRenderPass renderPass, uint32_t width, uint32_t height)
{
    VkImageView attachments[] = { colorAttachment, depthAttachment };

    VkFramebufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.renderPass = renderPass;
	createInfo.attachmentCount = 2;
	createInfo.pAttachments = attachments;
	createInfo.width = width;
	createInfo.height = height;
	createInfo.layers = 1;

    VkFramebuffer frameBuffer = nullptr;
    SL_CHECK_VK_RESULT(vkCreateFramebuffer(logicalDevice, &createInfo, nullptr, &frameBuffer));

    return frameBuffer;
}


void VulkanRenderer::initSwapchain(VkSurfaceKHR surface, bool vsync)
{
	VkSurfaceCapabilitiesKHR capabilities;
	SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));

    uint32_t presentModeCount;
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr));

    std::vector<VkPresentModeKHR> presentModes;
    presentModes.resize(presentModeCount);
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data()));

    if (capabilities.currentExtent.width == -1)
    {
        // Surface extent not defined - select based on device canvas size
        auto deviceCanvasSize = device->getCanvasSize();
        canvasWidth = static_cast<uint32_t>(deviceCanvasSize.x);
        canvasHeight = static_cast<uint32_t>(deviceCanvasSize.y);
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
    SL_CHECK_VK_RESULT(vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, nullptr));

    std::vector<VkImage> images;
    images.resize(imageCount);
    SL_CHECK_VK_RESULT(vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, images.data()));

    swapchainBuffers.resize(imageCount);
    for (uint32_t i = 0; i < imageCount; i++)
    {
        VkImageViewCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageInfo.pNext = nullptr;
		imageInfo.format = colorFormat;
		imageInfo.components = 
        {
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

        SL_CHECK_VK_RESULT(vkCreateImageView(logicalDevice, &imageInfo, nullptr, &swapchainBuffers[i].imageView));
    }
}


void VulkanRenderer::cleanupSwapchain()
{
    for (const auto& buf: swapchainBuffers)
        vkDestroyImageView(logicalDevice, buf.imageView, nullptr);
    vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);
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

    SL_CHECK_VK_RESULT(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, drawCmdBuffers.data()));
    SL_CHECK_VK_RESULT(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, prePresentCmdBuffers.data()));
    SL_CHECK_VK_RESULT(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, postPresentCmdBuffers.data()));

    initPresentationCommandBuffers();
}


void VulkanRenderer::initPresentationCommandBuffers()
{
    for (uint32_t i = 0; i < swapchainBuffers.size(); i++)
    {
        // Transform the image back to a color attachment that our render pass can write to

        beginCommandBuffer(postPresentCmdBuffers[i]);

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
        flushCommandBuffer(queue, postPresentCmdBuffers[i]);

        // Transforms the (framebuffer) image layout from color attachment to present(khr) for presenting to the swap chain

        beginCommandBuffer(prePresentCmdBuffers[i]);
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

        flushCommandBuffer(queue, prePresentCmdBuffers[i]);
    }
}


void VulkanRenderer::buildDrawCommandBuffers()
{
    VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
	cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufferBeginInfo.pNext = nullptr;

    VkClearValue clearValues[2];
	clearValues[0].color = { { 0.0f, 0.8f, 0.8f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent.width = canvasWidth;
	renderPassBeginInfo.renderArea.extent.height = canvasHeight;
	renderPassBeginInfo.clearValueCount = 2;
	renderPassBeginInfo.pClearValues = clearValues;

    VkViewport viewport = {};
	viewport.width = canvasWidth;
	viewport.height = canvasHeight;
	viewport.minDepth = 0;
	viewport.maxDepth = 1;

    VkRect2D scissor = {};
	scissor.extent.width = canvasWidth;
	scissor.extent.height = canvasHeight;
	scissor.offset.x = 0;
	scissor.offset.y = 0;

    for (int32_t i = 0; i < drawCmdBuffers.size(); ++i)
	{
		renderPassBeginInfo.framebuffer = frameBuffers[i];

		SL_CHECK_VK_RESULT(vkBeginCommandBuffer(drawCmdBuffers[i], &cmdBufferBeginInfo));

		vkCmdBeginRenderPass(drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdSetViewport(drawCmdBuffers[i], 0, 1, &viewport);
		vkCmdSetScissor(drawCmdBuffers[i], 0, 1, &scissor);

        // TODO scene rendering here

		vkCmdEndRenderPass(drawCmdBuffers[i]);

		SL_CHECK_VK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
	}
}


void VulkanRenderer::destroyCommandBuffers()
{
    ::destroyCommandBuffers(logicalDevice, commandPool, drawCmdBuffers.data(), drawCmdBuffers.size());
    ::destroyCommandBuffers(logicalDevice, commandPool, prePresentCmdBuffers.data(), prePresentCmdBuffers.size());
    ::destroyCommandBuffers(logicalDevice, commandPool, postPresentCmdBuffers.data(), postPresentCmdBuffers.size());
}


void VulkanRenderer::initFrameBuffers()
{
    frameBuffers.resize(swapchainBuffers.size());
    for (auto i = 0; i < swapchainBuffers.size(); i++)
        frameBuffers[i] = ::createFrameBuffer(logicalDevice, swapchainBuffers[i].imageView, depthStencil.view, renderPass, canvasWidth, canvasHeight);
}


// TODO remove these
static bool initialized = false;
struct Vertex
{
    float position[2];
    float color[3];
};


void VulkanRenderer::test_init()
{
    buildDrawCommandBuffers();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = 0;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    VkPipelineLayout pipelineLayout;
    SL_CHECK_VK_RESULT(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout));

    auto vertexShader = createShader(logicalDevice, Device::get()->getFileSystem()->readBytes("triangle.vert.spv"));
    auto fragmentShader = createShader(logicalDevice, Device::get()->getFileSystem()->readBytes("triangle.frag.spv"));

    VkPipelineShaderStageCreateInfo vertexShaderStage {};
    vertexShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStage.pNext = nullptr;
    vertexShaderStage.flags = 0;
    vertexShaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStage.module = vertexShader;
    vertexShaderStage.pName = "main";
    vertexShaderStage.pSpecializationInfo = nullptr;

    VkPipelineShaderStageCreateInfo fragmentShaderStage {};
    fragmentShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStage.pNext = nullptr;
    fragmentShaderStage.flags = 0;
    fragmentShaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStage.module = fragmentShader;
    fragmentShaderStage.pName = "main";
    fragmentShaderStage.pSpecializationInfo = nullptr;

    std::vector<VkPipelineShaderStageCreateInfo> shaderStageStates = { vertexShaderStage, fragmentShaderStage };

    VkVertexInputBindingDescription vertexInputBindingDesc {};
    vertexInputBindingDesc.binding = 0;
    vertexInputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vertexInputBindingDesc.stride = sizeof(Vertex);
    std::vector<VkVertexInputBindingDescription> inputBindingDescriptions = { vertexInputBindingDesc };

    const uint32_t positionLocation = 0;
	const uint32_t colorLocation = 1;

    VkVertexInputAttributeDescription positionInputAttrDesc {};
    positionInputAttrDesc.location = positionLocation;
    positionInputAttrDesc.binding = 0;
    positionInputAttrDesc.format = VK_FORMAT_R32G32_SFLOAT;
    positionInputAttrDesc.offset = offsetof(Vertex, position);

    VkVertexInputAttributeDescription colorInputAttrDesc {};
    colorInputAttrDesc.location = colorLocation;
    colorInputAttrDesc.binding = 0;
    colorInputAttrDesc.format = VK_FORMAT_R32G32_SFLOAT;
    colorInputAttrDesc.offset = offsetof(Vertex, color);

    std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions =
        { positionInputAttrDesc, colorInputAttrDesc };

    VkPipelineVertexInputStateCreateInfo vertexInputState {};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.pNext = nullptr;
    vertexInputState.flags = 0;
    vertexInputState.vertexBindingDescriptionCount = static_cast<uint32_t>(inputBindingDescriptions.size());
    vertexInputState.pVertexBindingDescriptions = inputBindingDescriptions.data();
    vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(inputAttributeDescriptions.size());
    vertexInputState.pVertexAttributeDescriptions = inputAttributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState {};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.pNext = nullptr;
    inputAssemblyState.flags = 0;
    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyState.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;
    viewportState.flags = 0;
    viewportState.viewportCount = 1;
    viewportState.pViewports = nullptr;
    viewportState.scissorCount = 1;
    viewportState.pScissors = nullptr;

    VkPipelineRasterizationStateCreateInfo rasterizationState {};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.pNext = nullptr;
    rasterizationState.flags = 0;
    rasterizationState.depthClampEnable = false;
    rasterizationState.rasterizerDiscardEnable = false;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationState.depthBiasEnable = false;
    rasterizationState.depthBiasClamp = 0;
    rasterizationState.depthBiasConstantFactor = 0;
    rasterizationState.depthBiasClamp = 0;
    rasterizationState.depthBiasSlopeFactor = 0;
    rasterizationState.lineWidth = 0;

    VkPipelineMultisampleStateCreateInfo multisampleState {};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.pNext = nullptr;
    multisampleState.flags = 0;
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleState.sampleShadingEnable = false;
    multisampleState.minSampleShading = 0;
    multisampleState.pSampleMask = nullptr;
    multisampleState.alphaToCoverageEnable = false;
    multisampleState.alphaToOneEnable = false;

    VkPipelineColorBlendAttachmentState blendAttachmentState {};
    blendAttachmentState.blendEnable = VK_FALSE;
    blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
    blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlendState {};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.pNext = nullptr;
    colorBlendState.flags = 0;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.logicOp = VK_LOGIC_OP_COPY;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &blendAttachmentState;
    colorBlendState.blendConstants[0] = 0;
    colorBlendState.blendConstants[1] = 0;
    colorBlendState.blendConstants[2] = 0;
    colorBlendState.blendConstants[3] = 0;
}


void VulkanRenderer::test_draw()
{
    if (!initialized)
    {
        test_init();
        initialized = true;
    }

    VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
    submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];

    SL_CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, nullptr));
}


VulkanRenderer::DepthStencil VulkanRenderer::createDepthStencil(
    VkDevice logicalDevice, VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
    VkCommandBuffer cmdBuffer, VkFormat depthFormat, uint32_t canvasWidth, uint32_t canvasHeight)
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

    DepthStencil depthStencil {};

    VkMemoryRequirements memReqs;
	SL_CHECK_VK_RESULT(vkCreateImage(logicalDevice, &image, nullptr, &depthStencil.image));
	vkGetImageMemoryRequirements(logicalDevice, depthStencil.image, &memReqs);

    auto memTypeIndex = findMemoryType(physicalDeviceMemoryProperties, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    SL_ERR_IF(memTypeIndex < 0);

	alloc.allocationSize = memReqs.size;
    alloc.memoryTypeIndex = memTypeIndex;
	SL_CHECK_VK_RESULT(vkAllocateMemory(logicalDevice, &alloc, nullptr, &depthStencil.mem));
    SL_CHECK_VK_RESULT(vkBindImageMemory(logicalDevice, depthStencil.image, depthStencil.mem, 0));
    
    setImageLayout(cmdBuffer, depthStencil.image, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    createInfo.image = depthStencil.image;
    SL_CHECK_VK_RESULT(vkCreateImageView(logicalDevice, &createInfo, nullptr, &depthStencil.view));

    return depthStencil;
}


VulkanRenderer::VulkanRenderer(Device* engineDevice):
    device(dynamic_cast<SDLVulkanDevice*>(engineDevice))
{
    auto instance = device->getVulkanInstance();
    auto surface = device->getVulkanSurface();

    physicalDevice = createPhysicalDevice(instance);
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);

    auto surfaceFormats = getSurfaceFormats(physicalDevice, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);

    auto queueIndex = getQueueIndex(physicalDevice, surface);

    logicalDevice = createLogicalDevice(physicalDevice, queueIndex);

    vkGetDeviceQueue(logicalDevice, queueIndex, 0, &queue);

    depthFormat = getDepthFormat(physicalDevice);

    presentCompleteSem = createSemaphore(logicalDevice);
    renderCompleteSem = createSemaphore(logicalDevice);

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &presentCompleteSem;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderCompleteSem;

    commandPool = createCommandPool(logicalDevice, queueIndex);
    initSwapchain(surface, device->getSetup().vsync);
    initCommandBuffers();

    setupCmdBuffer = createCommandBuffer(logicalDevice, commandPool);
    beginCommandBuffer(setupCmdBuffer);

    depthStencil = createDepthStencil(logicalDevice, physicalDeviceMemoryProperties, setupCmdBuffer, depthFormat, canvasWidth, canvasHeight);
    renderPass = createRenderPass(logicalDevice, colorFormat, depthFormat);
    initFrameBuffers();

    flushCommandBuffer(queue, setupCmdBuffer);
    ::destroyCommandBuffers(logicalDevice, commandPool, &setupCmdBuffer, 1);
}


VulkanRenderer::~VulkanRenderer()
{
    // TODO clean lots of other stuff
    destroyCommandBuffers();
    cleanupSwapchain();
}


void VulkanRenderer::beginFrame()
{
    SL_CHECK_VK_RESULT(vkAcquireNextImageKHR(logicalDevice, swapchain, UINT64_MAX, presentCompleteSem, nullptr, &currentBuffer));

    VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
    submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &postPresentCmdBuffers[currentBuffer];

    SL_CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, nullptr));

    test_draw();
}


void VulkanRenderer::endFrame()
{
    VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
    submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &prePresentCmdBuffers[currentBuffer];
    SL_CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, nullptr));

    VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.pImageIndices = &currentBuffer;
    presentInfo.pWaitSemaphores = &renderCompleteSem;
    presentInfo.waitSemaphoreCount = 1;
    SL_CHECK_VK_RESULT(vkQueuePresentKHR(queue, &presentInfo));

    SL_CHECK_VK_RESULT(vkQueueWaitIdle(queue));
}


#else
#   error Vulkan renderer is not supported on this platform
#endif