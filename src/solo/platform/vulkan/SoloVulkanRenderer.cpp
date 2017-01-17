/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloVulkanRenderer.h"
#include "SoloSDLVulkanDevice.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;


static auto createRenderCmdBuffers(uint32_t count, VkDevice device, VkCommandPool commandPool) -> std::vector<VkCommandBuffer>
{
    std::vector<VkCommandBuffer> buffers(count);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(count);

    SL_CHECK_VK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, buffers.data()));

    return buffers;
}


VulkanRenderer::VulkanRenderer(Device *engineDevice):
    canvasWidth(engineDevice->getSetup().canvasWidth),
    canvasHeight(engineDevice->getSetup().canvasHeight)
{
    auto vulkanDevice = dynamic_cast<SDLVulkanDevice*>(engineDevice);
    auto instance = vulkanDevice->getVkInstance();
    auto surface = vulkanDevice->getVkSurface();

    physicalDevice = vk::getPhysicalDevice(instance);
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &features);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    auto surfaceFormats = vk::getSurfaceFormats(physicalDevice, surface);
    colorFormat = std::get<0>(surfaceFormats);
    colorSpace = std::get<1>(surfaceFormats);

    auto queueIndex = vk::getQueueIndex(physicalDevice, surface);

    device = vk::createDevice(physicalDevice, queueIndex);

    vkGetDeviceQueue(device, queueIndex, 0, &queue);

    depthFormat = vk::getDepthFormat(physicalDevice);
    presentCompleteSem = vk::createSemaphore(device);
    renderCompleteSem = vk::createSemaphore(device);
    commandPool = vk::createCommandPool(device, queueIndex);
    depthStencil = vk::createDepthStencil(device, memProperties, depthFormat, canvasWidth, canvasHeight);
    renderPass = vk::createRenderPass(device, colorFormat, depthFormat);
    
    initSwapchain(surface, engineDevice->getSetup().vsync);
    initFrameBuffers();
    
    renderCmdBuffers = createRenderCmdBuffers(swapchainBuffers.size(), device, commandPool);

    // TODO remove
    buildCommandBuffers();
}


VulkanRenderer::~VulkanRenderer()
{
    vkFreeCommandBuffers(device, commandPool, renderCmdBuffers.size(), renderCmdBuffers.data());

    for (size_t i = 0; i < frameBuffers.size(); ++i)
        vkDestroyFramebuffer(device, frameBuffers[i], nullptr);

    for (auto &buf : swapchainBuffers)
        vkDestroyImageView(device, buf.imageView, nullptr);
    vkDestroySwapchainKHR(device, swapchain, nullptr);

    // Render pass
    vkDestroyRenderPass(device, renderPass, nullptr);

    // Depth stencil
    if (depthStencil.view)
        vkDestroyImageView(device, depthStencil.view, nullptr);
    if (depthStencil.image)
        vkDestroyImage(device, depthStencil.image, nullptr);
    if (depthStencil.mem)
        vkFreeMemory(device, depthStencil.mem, nullptr);

    // Command pool
    vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroySemaphore(device, presentCompleteSem, nullptr);
    vkDestroySemaphore(device, renderCompleteSem, nullptr);

    vkDestroyDevice(device, nullptr);
}


void VulkanRenderer::beginFrame()
{
    SL_CHECK_VK_RESULT(vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, presentCompleteSem, nullptr, &currentBuffer));
}


void VulkanRenderer::endFrame()
{
    VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &presentCompleteSem;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderCompleteSem;
    submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &renderCmdBuffers[currentBuffer];
    SL_CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

    VkPresentInfoKHR presentInfo{};
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


void VulkanRenderer::initSwapchain(VkSurfaceKHR surface, bool vsync)
{
    VkSurfaceCapabilitiesKHR capabilities;
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));

    uint32_t presentModeCount;
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr));

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data()));

    auto width = static_cast<uint32_t>(this->canvasWidth);
    auto height = static_cast<uint32_t>(this->canvasHeight);
    if (capabilities.currentExtent.width != static_cast<uint32_t>(-1))
    {
        width = capabilities.currentExtent.width;
        height = capabilities.currentExtent.height;
    }

    auto presentMode = VK_PRESENT_MODE_FIFO_KHR; // "vsync"

    if (!vsync)
    {
        for (const auto mode : presentModes)
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

    VkSwapchainCreateInfoKHR swapchainInfo{};
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.pNext = nullptr;
    swapchainInfo.surface = surface;
    swapchainInfo.minImageCount = requestedImageCount;
    swapchainInfo.imageFormat = colorFormat;
    swapchainInfo.imageColorSpace = colorSpace;
    swapchainInfo.imageExtent = {width, height};
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

    SL_CHECK_VK_RESULT(vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, &swapchain));

    uint32_t imageCount = 0;
    SL_CHECK_VK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));

    std::vector<VkImage> images;
    images.resize(imageCount);
    SL_CHECK_VK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()));

    swapchainBuffers.resize(imageCount);

    for (uint32_t i = 0; i < imageCount; i++)
    {
        VkImageViewCreateInfo imageViewInfo{};
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

        SL_CHECK_VK_RESULT(vkCreateImageView(device, &imageViewInfo, nullptr, &swapchainBuffers[i].imageView));
    }
}


void VulkanRenderer::initFrameBuffers()
{
    auto count = swapchainBuffers.size();
    frameBuffers.resize(count);
    for (auto i = 0; i < count; i++)
        frameBuffers[i] = vk::createFrameBuffer(device, swapchainBuffers[i].imageView, depthStencil.view, renderPass, canvasWidth, canvasHeight);
}


void VulkanRenderer::buildCommandBuffers()
{
    static VkClearColorValue defaultClearColor = {{0.5f, 0.1f, 0.1f, 1.0f}};

    // Build

    VkViewport viewport {};
	viewport.width = canvasWidth;
	viewport.height = canvasHeight;
	viewport.minDepth = 0;
	viewport.maxDepth = 1;

    VkRect2D scissor {};
    scissor.extent.width = canvasWidth;
    scissor.extent.height = canvasHeight;
    scissor.offset.x = 0;
    scissor.offset.y = 0;

    VkClearValue clearValues[2];
	clearValues[0].color = defaultClearColor;
    clearValues[1].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent.width = canvasWidth;
	renderPassBeginInfo.renderArea.extent.height = canvasHeight;
	renderPassBeginInfo.clearValueCount = 2;
	renderPassBeginInfo.pClearValues = clearValues;

    // TODO this is only for testing
    for (size_t i = 0; i < renderCmdBuffers.size(); i++)
    {
        renderPassBeginInfo.framebuffer = frameBuffers[i];

        vk::recordCommandBuffer(renderCmdBuffers[i], [=](VkCommandBuffer buf)
        {
            vkCmdBeginRenderPass(buf, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdSetViewport(renderCmdBuffers[i], 0, 1, &viewport);
            vkCmdSetScissor(renderCmdBuffers[i], 0, 1, &scissor);

            vkCmdEndRenderPass(buf);
        });
    }
}

#endif
