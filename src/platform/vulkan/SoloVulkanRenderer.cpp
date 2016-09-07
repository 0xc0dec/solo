// Note: this file is a total hackery and a work-in-progress, so avoid reading this

#include "SoloVulkanRenderer.h"
#include "SoloVulkanPipeline.h"
#include "SoloVulkanBuffer.h"
#include "SoloSDLVulkanDevice.h"
#include "SoloFileSystem.h" // TODO remove?

#ifdef SL_VULKAN_RENDERER

using namespace solo;


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
        VkImageViewCreateInfo imageViewInfo = {};
        imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.pNext = nullptr;
        imageViewInfo.format = colorFormat;
        imageViewInfo.components = 
        {
            VK_COMPONENT_SWIZZLE_R,
            VK_COMPONENT_SWIZZLE_G,
            VK_COMPONENT_SWIZZLE_B,
            VK_COMPONENT_SWIZZLE_A
        };
        imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewInfo.subresourceRange.baseMipLevel = 0;
        imageViewInfo.subresourceRange.levelCount = 1;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = 1;
        imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewInfo.flags = 0;
        imageViewInfo.image = images[i];

        swapchainBuffers[i].image = images[i];

        SL_CHECK_VK_RESULT(vkCreateImageView(logicalDevice, &imageViewInfo, nullptr, &swapchainBuffers[i].imageView));
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
        VulkanHelper::submitCommandBuffer(queue, postPresentCmdBuffers[i]);

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

        VulkanHelper::submitCommandBuffer(queue, prePresentCmdBuffers[i]);
    }
}


void VulkanRenderer::buildDrawCommandBuffers(std::function<void(VkCommandBuffer)> meat /* TODO removeme */)
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

        meat(drawCmdBuffers[i]);
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
        frameBuffers[i] = VulkanHelper::createFrameBuffer(logicalDevice, swapchainBuffers[i].imageView, depthStencil.view, renderPass, canvasWidth, canvasHeight);
}


// TODO remove these
static bool initialized = false;
VkPipeline pipeline = nullptr;
struct Vertex
{
    float position[2];
    float color[3];
};


void VulkanRenderer::test_init()
{
    auto vertexShader = VulkanHelper::createShader(logicalDevice, Device::get()->getFileSystem()->readBytes("triangle.vert.spv"));
    auto fragmentShader = VulkanHelper::createShader(logicalDevice, Device::get()->getFileSystem()->readBytes("triangle.frag.spv"));

    VulkanPipeline pipeline(logicalDevice, renderPass);
    pipeline.setVertexShader(vertexShader, "main");
    pipeline.setFragmentShader(fragmentShader, "main");

    pipeline.setVertexSize(sizeof(Vertex));

    pipeline.setVertexAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position));
    pipeline.setVertexAttribute(1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, color));

    pipeline.setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    pipeline.setCullMode(VK_CULL_MODE_BACK_BIT);
    pipeline.setFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
    pipeline.setRasterizationSampleCount(VK_SAMPLE_COUNT_1_BIT);
    pipeline.setColorBlend(VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD);
    pipeline.setAlphaBlend(VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD);
    pipeline.setColorWriteMask(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

    pipeline.rebuild();

    const auto triangleSize = 1.6f;
    std::vector<Vertex> triangle =
    {
		{ /*rb*/ {  0.5f * triangleSize,  sqrtf( 3.0f ) * 0.25f * triangleSize }, /*R*/{ 1.0f, 0.0f, 0.0f }},
		{ /* t*/ {                 0.0f, -sqrtf( 3.0f ) * 0.25f * triangleSize }, /*G*/{ 0.0f, 1.0f, 0.0f }},
		{ /*lb*/ { -0.5f * triangleSize,  sqrtf( 3.0f ) * 0.25f * triangleSize }, /*B*/{ 0.0f, 0.0f, 1.0f }}
	};

    std::vector<Vertex> triangle2 =
    {
		{ /*rb*/ {  0.3f * triangleSize,  sqrtf(3.0f) * 0.25f * triangleSize }, /*R*/{ 1.0f, 0.0f, 0.0f }},
		{ /* t*/ {                 0.0f, -sqrtf(3.0f) * 0.25f * triangleSize }, /*G*/{ 0.0f, 1.0f, 0.0f }},
		{ /*lb*/ { -0.5f * triangleSize,  sqrtf(3.0f) * 0.25f * triangleSize }, /*B*/{ 0.0f, 0.0f, 1.0f }}
	};
    auto vertexBuffer = VulkanBuffer::create(logicalDevice, triangle.data(), sizeof(decltype(triangle)::value_type) * triangle.size(),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        physicalDeviceMemoryProperties);

    buildDrawCommandBuffers([&](VkCommandBuffer buf)
    {
        vkCmdBindPipeline(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getHandle());

        VkDeviceSize offsets[] = {0};
        auto& buffer = vertexBuffer.getHandle();
	    vkCmdBindVertexBuffers(buf, 0, 1, &buffer, offsets);

        vkCmdDraw(buf, static_cast<uint32_t>(triangle.size()), 1, 0, 0);
    });
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

    auto memTypeIndex = VulkanHelper::findMemoryType(physicalDeviceMemoryProperties, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    SL_ERR_IF(memTypeIndex < 0);

    alloc.allocationSize = memReqs.size;
    alloc.memoryTypeIndex = memTypeIndex;
    SL_CHECK_VK_RESULT(vkAllocateMemory(logicalDevice, &alloc, nullptr, &depthStencil.mem));
    SL_CHECK_VK_RESULT(vkBindImageMemory(logicalDevice, depthStencil.image, depthStencil.mem, 0));
    
    VulkanHelper::setImageLayout(cmdBuffer, depthStencil.image, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
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

    physicalDevice = VulkanHelper::createPhysicalDevice(instance);
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);

    auto surfaceFormats = getSurfaceFormats(physicalDevice, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);

    auto queueIndex = VulkanHelper::getQueueIndex(physicalDevice, surface);

    logicalDevice = VulkanHelper::createLogicalDevice(physicalDevice, queueIndex);

    vkGetDeviceQueue(logicalDevice, queueIndex, 0, &queue);

    depthFormat = VulkanHelper::getDepthFormat(physicalDevice);

    presentCompleteSem = createSemaphore(logicalDevice);
    renderCompleteSem = createSemaphore(logicalDevice);

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &presentCompleteSem;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderCompleteSem;

    commandPool = VulkanHelper::createCommandPool(logicalDevice, queueIndex);
    initSwapchain(surface, device->getSetup().vsync);
    initCommandBuffers();

    setupCmdBuffer = VulkanHelper::createCommandBuffer(logicalDevice, commandPool);
    beginCommandBuffer(setupCmdBuffer);

    depthStencil = createDepthStencil(logicalDevice, physicalDeviceMemoryProperties, setupCmdBuffer, depthFormat, canvasWidth, canvasHeight);
    renderPass = VulkanHelper::createRenderPass(logicalDevice, colorFormat, depthFormat);
    initFrameBuffers();

    VulkanHelper::submitCommandBuffer(queue, setupCmdBuffer);
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