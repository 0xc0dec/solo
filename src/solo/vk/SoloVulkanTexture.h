/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloRectTexture.h"
#include "SoloVulkanImage.h"

namespace solo
{
    class Device;

    namespace vk
    {
        class Texture
        {
        public:
            auto getImage() const -> VkImageLayout { return image.getLayout(); }
            auto getView() const -> VkImageView { return image.getView(); }
            auto getSampler() const -> VkSampler { return image.getSampler(); }

        protected:
            Image image;
        };

        class RectTexture final: public solo::RectTexture, public vk::Texture
        {
        public:
            RectTexture(Device *device, uint32_t width, uint32_t height, TextureFormat format);

            void generateMipmaps() override final;
            void setData(const void *data) override final;
        };
    }
}

#endif