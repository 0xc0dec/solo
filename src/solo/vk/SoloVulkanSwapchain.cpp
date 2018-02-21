/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanSwapchain.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVulkanRenderer.h"
#include "SoloVulkanSDLDevice.h"

using namespace solo;

static auto getSwapchainImages(VkDevice device, VkSwapchainKHR swapchain) -> vec<VkImage>
{
    u32 imageCount = 0;
    SL_VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));
    
    vec<VkImage> images;
    images.resize(imageCount);
    SL_VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()));

    return images;
}

static auto getPresentMode(VulkanRenderer *renderer, VulkanSDLDevice *device, bool vsync) -> VkPresentModeKHR
{
    u32 presentModeCount;
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(renderer->getPhysicalDevice(), device->getSurface(), &presentModeCount, nullptr));

    vec<VkPresentModeKHR> presentModes(presentModeCount);
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

static auto createSwapchain(VulkanRenderer *renderer, VulkanSDLDevice *device, u32 width, u32 height, bool vsync) -> VulkanResource<VkSwapchainKHR>
{
    VkSurfaceCapabilitiesKHR capabilities;
    SL_VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(renderer->getPhysicalDevice(), device->getSurface(), &capabilities));

    if (capabilities.currentExtent.width != static_cast<u32>(-1))
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

    VulkanResource<VkSwapchainKHR> swapchain{renderer->getDevice(), vkDestroySwapchainKHR};
    SL_VK_CHECK_RESULT(vkCreateSwapchainKHR(renderer->getDevice(), &swapchainInfo, nullptr, swapchain.cleanRef()));

    return swapchain;
}

VulkanSwapchain::VulkanSwapchain(VulkanRenderer *renderer, VulkanSDLDevice *device, u32 width, u32 height, bool vsync):
    device(renderer->getDevice())
{
    const auto colorFormat = renderer->getColorFormat();
    const auto depthFormat = renderer->getDepthFormat();

    swapchain = createSwapchain(renderer, device, width, height, vsync);

    renderPass = VulkanRenderPass(this->device, VulkanRenderPassConfig()
        .withColorAttachment(colorFormat, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
        .withDepthAttachment(depthFormat));
    
    // TODO Replace with factory method?
    depthStencil = VulkanImage(renderer, width, height, 1, 1, depthFormat,
        VK_IMAGE_LAYOUT_UNDEFINED,
        0,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);

    auto images = getSwapchainImages(this->device, swapchain);

    const auto cmdBuf = vk::createCommandBuffer(renderer->getDevice(), renderer->getCommandPool(), true);

    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;
    
    steps.resize(images.size());
    for (u32 i = 0; i < images.size(); i++)
    {
        // TODO Use FrameBuffer class here?
        auto view = vk::createImageView(this->device, colorFormat, VK_IMAGE_VIEW_TYPE_2D, 1, 1, images[i], VK_IMAGE_ASPECT_COLOR_BIT);
        steps[i].framebuffer = vk::createFrameBuffer(this->device, {view, depthStencil.getView()}, renderPass, width, height);
        steps[i].image = images[i];
        steps[i].imageView = std::move(view);

        vk::setImageLayout(
            cmdBuf,
            images[i],
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            subresourceRange);
    }

    vk::flushCommandBuffer(cmdBuf, renderer->getQueue());

    presentCompleteSem = vk::createSemaphore(this->device);
}

auto VulkanSwapchain::moveNext() -> VkSemaphore
{
    SL_VK_CHECK_RESULT(vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, presentCompleteSem, VK_NULL_HANDLE, &currentStep));
    return presentCompleteSem;
}

void VulkanSwapchain::present(VkQueue queue, u32 waitSemaphoreCount, const VkSemaphore *waitSemaphores)
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &currentStep;
    presentInfo.pWaitSemaphores = waitSemaphores;
    presentInfo.waitSemaphoreCount = waitSemaphoreCount;
    SL_VK_CHECK_RESULT(vkQueuePresentKHR(queue, &presentInfo));
}

#endif
