/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloVector2.h"
#include "SoloVulkan.h"

namespace solo
{
    class Texture2DData;
    class CubeTextureData;
    class VulkanDevice;
    enum class TextureFormat;

    class VulkanImage
    {
    public:
        static auto empty(const VulkanDevice &dev, u32 width, u32 height, TextureFormat format) -> VulkanImage;
        static auto fromData(const VulkanDevice &dev, Texture2DData *data, bool generateMipmaps) -> VulkanImage;
        static auto fromDataCube(const VulkanDevice &dev, CubeTextureData *data) -> VulkanImage;

        VulkanImage() = default;
        VulkanImage(const VulkanDevice &dev, u32 width, u32 height, u32 mipLevels, u32 layers, VkFormat format, VkImageLayout layout,
            VkImageCreateFlags createFlags, VkImageUsageFlags usageFlags, VkImageViewType viewType, VkImageAspectFlags aspectMask);
        VulkanImage(const VulkanImage &other) = delete;
        VulkanImage(VulkanImage &&other) = default;

        auto operator=(const VulkanImage &other) -> VulkanImage& = delete;
        auto operator=(VulkanImage &&other) -> VulkanImage& = default;

        auto format() const -> VkFormat { return format_;  }
        auto size() const -> Vector2 { return {static_cast<float>(width_), static_cast<float>(height_)}; }
        auto mipLevels() const -> u32 { return mipLevels_; }
        auto layout() const -> VkImageLayout { return layout_; }
        auto view() const -> VkImageView { return view_; }
        auto handle() const -> VkImage { return image_; }
        auto width() const -> u32 { return width_; }
        auto height() const -> u32 { return height_; }

    private:
        VulkanResource<VkImage> image_;
        VulkanResource<VkDeviceMemory> memory_;
        VulkanResource<VkImageView> view_;
        VkImageLayout layout_ = VK_IMAGE_LAYOUT_UNDEFINED;
        VkFormat format_ = VK_FORMAT_UNDEFINED;
        u32 mipLevels_ = 0;
        u32 width_ = 0;
        u32 height_ = 0;
        VkImageAspectFlags aspectMask_ = VK_IMAGE_ASPECT_COLOR_BIT;
    };
}

#endif
