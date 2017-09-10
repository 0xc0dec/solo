/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloTexture.h"
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

        class Texture2d final: public solo::Texture2d, public vk::Texture
        {
        public:
            Texture2d(Device *device, Texture2dData *data);

            void generateMipmaps() override final;
        };
    }
}

#endif