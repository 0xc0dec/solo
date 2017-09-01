/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanSwapchain.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanRenderer.h"
#include "SoloSDLVulkanDevice.h"

using namespace solo;
using namespace vk;

static auto getSwapchainImages(VkDevice device, VkSwapchainKHR swapchain) -> std::vector<VkImage>
{
    uint32_t imageCount = 0;
    SL_VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));
    
    std::vector<VkImage> images;
    images.resize(imageCount);
    SL_VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()));

    return images;
}

static auto getPresentMode(vk::Renderer *renderer, SDLDevice *device, bool vsync) -> VkPresentModeKHR
{
    uint32_t presentModeCount;
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(renderer->getPhysicalDevice(), device->getSurface(), &presentModeCount, nullptr));

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(renderer->getPhysicalDevice(), device->getSurface(), &presentModeCount, presentModes.data()));

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

    return presentMode;
}

static auto createSwapchain(vk::Renderer *renderer, SDLDevice *device, uint32_t width, uint32_t height, bool vsync) -> Resource<VkSwapchainKHR>
{
    VkSurfaceCapabilitiesKHR capabilities;
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(renderer->getPhysicalDevice(), device->getSurface(), &capabilities));

    if (capabilities.currentExtent.width != static_cast<uint32_t>(-1))
    {
        width = capabilities.currentExtent.width;
        height = capabilities.currentExtent.height;
    }

    VkSurfaceTransformFlagsKHR transformFlags;
    if (capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        transformFlags = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    else
        transformFlags = capabilities.currentTransform;

    auto requestedImageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && requestedImageCount > capabilities.maxImageCount)
        requestedImageCount = capabilities.maxImageCount;

    const auto presentMode = getPresentMode(renderer, device, vsync);

    VkSwapchainCreateInfoKHR swapchainInfo{};
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.pNext = nullptr;
    swapchainInfo.surface = device->getSurface();
    swapchainInfo.minImageCount = requestedImageCount;
    swapchainInfo.imageFormat = renderer->getColorFormat();
    swapchainInfo.imageColorSpace = renderer->getColorSpace();
    swapchainInfo.imageExtent = {width, height};
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainInfo.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(transformFlags);
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainInfo.queueFamilyIndexCount = 0;
    swapchainInfo.pQueueFamilyIndices = nullptr;
    swapchainInfo.presentMode = presentMode;
    swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
    swapchainInfo.clipped = VK_TRUE;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    Resource<VkSwapchainKHR> swapchain{renderer->getDevice(), vkDestroySwapchainKHR};
    SL_VK_CHECK_RESULT(vkCreateSwapchainKHR(renderer->getDevice(), &swapchainInfo, nullptr, swapchain.cleanRef()));

    return swapchain;
}

Swapchain::Swapchain(vk::Renderer *renderer, SDLDevice *device, uint32_t width, uint32_t height, bool vsync):
    device(renderer->getDevice())
{
    const auto colorFormat = renderer->getColorFormat();
    const auto depthFormat = renderer->getDepthFormat();

    swapchain = createSwapchain(renderer, device, width, height, vsync);

    renderPass = RenderPass(this->device, RenderPassConfig()
        .withColorAttachment(colorFormat, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, true, {0, 0, 0, 1})
        .withDepthAttachment(depthFormat, true, {1, 0}));
    
    depthStencil = Image(renderer, width, height, 1, 1, depthFormat,
        0,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);

    auto images = getSwapchainImages(this->device, swapchain);
    
    steps.resize(images.size());
    for (uint32_t i = 0; i < images.size(); i++)
    {
        auto view = createImageView(this->device, colorFormat, VK_IMAGE_VIEW_TYPE_2D, 1, 1, images[i], VK_IMAGE_ASPECT_COLOR_BIT);
        steps[i].framebuffer = createFrameBuffer(this->device, view, depthStencil.getView(), renderPass, width, height);
        steps[i].image = images[i];
        steps[i].imageView = std::move(view);
        steps[i].cmdBuffer = createCommandBuffer(this->device, renderer->getCommandPool());
    }

    presentCompleteSem = createSemaphore(this->device);
    renderCompleteSem = createSemaphore(this->device);
}

auto Swapchain::acquireNext() -> VkSemaphore
{
    SL_VK_CHECK_RESULT(vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, presentCompleteSem, VK_NULL_HANDLE, &nextStep));
    return presentCompleteSem;
}

void Swapchain::recordCommandBuffers(std::function<void(VkFramebuffer, VkCommandBuffer)> issueCommands)
{
    for (size_t i = 0; i < steps.size(); ++i)
    {
        VkCommandBuffer buf = steps[i].cmdBuffer;
        beginCommandBuffer(buf, false);
        issueCommands(steps[i].framebuffer, buf);
        SL_VK_CHECK_RESULT(vkEndCommandBuffer(buf));
    }
}

void Swapchain::presentNext(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore *waitSemaphores)
{
    queueSubmit(queue, waitSemaphoreCount, waitSemaphores, 1, &renderCompleteSem, 1, &steps[nextStep].cmdBuffer);

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &nextStep;
    presentInfo.pWaitSemaphores = &renderCompleteSem;
    presentInfo.waitSemaphoreCount = 1;
    SL_VK_CHECK_RESULT(vkQueuePresentKHR(queue, &presentInfo));
}

#endif