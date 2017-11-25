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

    class VulkanTexture
    {
    public:
        auto getImage() const -> VulkanImage const& { return image; }

    protected:
        VulkanImage image;
    };

    class VulkanTexture2d final: public Texture2d, public VulkanTexture
    {
    public:
        VulkanTexture2d(Device *device, Texture2dData *data);

        void generateMipmaps() override final;
    };
}

#endif