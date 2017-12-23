/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVector2.h"
#include "SoloVulkan.h"

namespace solo
{
    class Texture2dData;
    class CubeTextureData;
    class VulkanRenderer;

    class VulkanImage
    {
    public:
        static auto create2d(VulkanRenderer *renderer, Texture2dData *data) -> VulkanImage;
        static auto createCube(VulkanRenderer *renderer, CubeTextureData *data) -> VulkanImage;

        VulkanImage() {}
        VulkanImage(VulkanRenderer *renderer, u32 width, u32 height, u32 mipLevels, u32 layers, VkFormat format,
            VkImageCreateFlags createFlags, VkImageUsageFlags usageFlags, VkImageViewType viewType, VkImageAspectFlags aspectMask);
        VulkanImage(const VulkanImage &other) = delete;
        VulkanImage(VulkanImage &&other) = default;

        auto operator=(const VulkanImage &other) -> VulkanImage& = delete;
        auto operator=(VulkanImage &&other) -> VulkanImage& = default;

        auto getFormat() const -> VkFormat { return format;  }
        auto getSize() const -> Vector2 { return {static_cast<float>(width), static_cast<float>(height)}; }
        auto getLayout() const -> VkImageLayout { return layout; }
        auto getView() const -> VkImageView { return view; }

    private:
        VulkanResource<VkImage> image;
        VulkanResource<VkDeviceMemory> memory;
        VulkanResource<VkImageView> view;
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkFormat format = VK_FORMAT_UNDEFINED;
        u32 mipLevels = 0;
        u32 layers = 0;
        u32 width = 0;
        u32 height = 0;
        VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    };
}

#endif
