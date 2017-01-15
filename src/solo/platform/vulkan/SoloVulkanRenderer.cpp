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
#include "SoloVulkanPipeline.h"
#include "SoloVector2.h"
// TODO remove
#include "SoloFileSystem.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;


auto VulkanRenderer::createDepthStencil(VkDevice device, VkPhysicalDeviceMemoryProperties physicalDeviceMemProps,
    VkFormat depthFormat, uint32_t canvasWidth, uint32_t canvasHeight) -> DepthStencil
{
    VkImageCreateInfo imageInfo {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = nullptr;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = depthFormat;
    imageInfo.extent = {canvasWidth, canvasHeight, 1};
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    imageInfo.flags = 0;

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = 0;
    allocInfo.memoryTypeIndex = 0;

    VkImageViewCreateInfo viewInfo {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.pNext = nullptr;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = depthFormat;
    viewInfo.flags = 0;
    viewInfo.subresourceRange = {};
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    DepthStencil depthStencil;

    VkMemoryRequirements memReqs;
    SL_CHECK_VK_RESULT(vkCreateImage(device, &imageInfo, nullptr, &depthStencil.image));
    vkGetImageMemoryRequirements(device, depthStencil.image, &memReqs);

    auto memTypeIndex = vk::findMemoryType(physicalDeviceMemProps, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    SL_ERR_IF(memTypeIndex < 0);

    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = memTypeIndex;
    SL_CHECK_VK_RESULT(vkAllocateMemory(device, &allocInfo, nullptr, &depthStencil.mem));
    SL_CHECK_VK_RESULT(vkBindImageMemory(device, depthStencil.image, depthStencil.mem, 0));

    viewInfo.image = depthStencil.image;
    SL_CHECK_VK_RESULT(vkCreateImageView(device, &viewInfo, nullptr, &depthStencil.view));

    return depthStencil;
}


void VulkanRenderer::initFrameBuffers()
{
    frameBuffers.resize(swapchainBuffers.size());
    for (auto i = 0; i < swapchainBuffers.size(); i++)
        frameBuffers[i] = vk::createFrameBuffer(device, swapchainBuffers[i].imageView, depthStencil.view, renderPass, canvasWidth, canvasHeight);
}


// TODO this is only for testing
static SDLVulkanDevice *vulkanDevice = nullptr;


VulkanRenderer::VulkanRenderer(Device *engineDevice)
{
    vulkanDevice = dynamic_cast<SDLVulkanDevice*>(engineDevice);

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

    initSwapchain(surface, engineDevice->getSetup().vsync, engineDevice->getCanvasSize());
    initCommandBuffers();

    depthStencil = createDepthStencil(device, memProperties, depthFormat, canvasWidth, canvasHeight);

    renderPass = vk::createRenderPass(device, colorFormat, depthFormat);

    initFrameBuffers();
}


// TODO this is only for testing
static VulkanPipeline *pipeline = nullptr;
static bool init = false;


VulkanRenderer::~VulkanRenderer()
{
    vkDeviceWaitIdle(device);

    // TODO this is only for testing
    delete pipeline;

    for (size_t i = 0; i < swapchainBuffers.size(); ++i)
        vkDestroyImageView(device, swapchainBuffers[i].imageView, nullptr);

    if (presentCompleteSem)
        vkDestroySemaphore(device, presentCompleteSem, nullptr);
    if (renderCompleteSem)
        vkDestroySemaphore(device, renderCompleteSem, nullptr);

    destroySwapchain();

    vkFreeCommandBuffers(device, commandPool, drawCmdBuffers.size(), drawCmdBuffers.data());

    if (renderPass)
        vkDestroyRenderPass(device, renderPass, nullptr);

    for (size_t i = 0; i < frameBuffers.size(); ++i)
        vkDestroyFramebuffer(device, frameBuffers[i], nullptr);

    if (depthStencil.view)
        vkDestroyImageView(device, depthStencil.view, nullptr);
    if (depthStencil.image)
        vkDestroyImage(device, depthStencil.image, nullptr);
    if (depthStencil.mem)
        vkFreeMemory(device, depthStencil.mem, nullptr);

    if (commandPool)
        vkDestroyCommandPool(device, commandPool, nullptr);

    if (device)
        vkDestroyDevice(device, nullptr);
}


void VulkanRenderer::beginFrame()
{
    // TODO this is only for testing
    if (!init)
    {
        pipeline = new VulkanPipeline(device, renderPass);
        auto vertShader = vk::createShader(device, vulkanDevice->getFileSystem()->readBytes("../assets/triangle.vert.spv"));
        auto fragShader = vk::createShader(device, vulkanDevice->getFileSystem()->readBytes("../assets/triangle.frag.spv"));
        pipeline->setVertexShader(vertShader, "main");
        pipeline->setFragmentShader(fragShader, "main");
        pipeline->rebuild();
        init = true;
    }

    SL_CHECK_VK_RESULT(vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, presentCompleteSem, nullptr, &currentBuffer));
}


void VulkanRenderer::endFrame()
{
    VkPresentInfoKHR presentInfo;
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


void VulkanRenderer::initSwapchain(VkSurfaceKHR surface, bool vsync, const Vector2 &deviceCanvasSize)
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

    VkSwapchainCreateInfoKHR swapchainInfo = {};
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.pNext = nullptr;
    swapchainInfo.surface = surface;
    swapchainInfo.minImageCount = requestedImageCount;
    swapchainInfo.imageFormat = colorFormat;
    swapchainInfo.imageColorSpace = colorSpace;
    swapchainInfo.imageExtent = {canvasWidth, canvasHeight};
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
    vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, &swapchain);

    uint32_t imageCount = 0;
    SL_CHECK_VK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));

    std::vector<VkImage> images;
    images.resize(imageCount);
    SL_CHECK_VK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()));

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

        SL_CHECK_VK_RESULT(vkCreateImageView(device, &imageViewInfo, nullptr, &swapchainBuffers[i].imageView));
    }
}


void VulkanRenderer::destroySwapchain()
{
    for (auto &buf : swapchainBuffers)
        vkDestroyImageView(device, buf.imageView, nullptr);
}


void VulkanRenderer::initCommandBuffers()
{
    auto count = swapchainBuffers.size();

    drawCmdBuffers.resize(count);

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(count);

    SL_CHECK_VK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, drawCmdBuffers.data()));
}


void VulkanRenderer::beginCommandBuffer(VkCommandBuffer buffer)
{
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    SL_CHECK_VK_RESULT(vkBeginCommandBuffer(buffer, &beginInfo));
}

#endif
