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

    namespace vk
    {
        class Renderer;

        class Image
        {
        public:
            static auto create2d(Renderer *renderer, Texture2dData *data) -> Image;
            static auto createCube(Renderer *renderer/*, const ImageData &data*/) -> Image;

            Image() {}
            Image(Renderer *renderer, u32 width, u32 height, u32 mipLevels, u32 layers, VkFormat format,
                VkImageCreateFlags createFlags, VkImageUsageFlags usageFlags, VkImageViewType viewType, VkImageAspectFlags aspectMask);
            Image(const Image &other) = delete;
            Image(Image &&other) = default;

            auto operator=(const Image &other) -> Image& = delete;
            auto operator=(Image &&other) -> Image& = default;

            auto getSize() const -> Vector2 { return {static_cast<float>(width), static_cast<float>(height)}; }
            auto getLayout() const -> VkImageLayout { return layout; }
            auto getSampler() const -> VkSampler { return sampler; }
            auto getView() const -> VkImageView { return view; }

        private:
            Resource<VkImage> image;
            Resource<VkDeviceMemory> memory;
            Resource<VkImageView> view;
            Resource<VkSampler> sampler;
            VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
            u32 mipLevels = 0;
            u32 layers = 0;
            u32 width = 0;
            u32 height = 0;
            VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        };
    }
}

#endif
