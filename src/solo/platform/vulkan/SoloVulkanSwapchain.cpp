#include "SoloVulkanSwapchain.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;


VulkanSwapchain::VulkanSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
    bool vsync, const Vector2 &deviceCanvasSize, VkFormat colorFormat, VkColorSpaceKHR colorSpace):
    device(device)
{
    VkSurfaceCapabilitiesKHR capabilities;
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));

    uint32_t presentModeCount;
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr));

    std::vector<VkPresentModeKHR> presentModes;
    presentModes.resize(presentModeCount);
    SL_CHECK_VK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data()));

    if (capabilities.currentExtent.width == static_cast<uint32_t>(-1))
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

    buffers.resize(imageCount);
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

        buffers[i].image = images[i];

        SL_CHECK_VK_RESULT(vkCreateImageView(device, &imageViewInfo, nullptr, &buffers[i].imageView));
    }
}


VulkanSwapchain::~VulkanSwapchain()
{
    for (auto &buf : buffers)
        vkDestroyImageView(device, buf.imageView, nullptr);
}


auto VulkanSwapchain::acquireNextImage(VkSemaphore presentCompleteSem) const -> uint32_t
{
    uint32_t index = 0;
    SL_CHECK_VK_RESULT(vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, presentCompleteSem, nullptr, &index));
    return index;
}


void VulkanSwapchain::present(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSem) const
{
    VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pWaitSemaphores = &waitSem;
	presentInfo.waitSemaphoreCount = 1;

    SL_CHECK_VK_RESULT(vkQueuePresentKHR(queue, &presentInfo));
}

#endif
