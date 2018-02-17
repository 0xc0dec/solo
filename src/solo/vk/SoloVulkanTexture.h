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
        auto getSampler() const -> VkSampler { return sampler; }

    protected:
        VulkanRenderer *renderer = nullptr;
        VulkanImage image;
        VulkanResource<VkSampler> sampler;

        explicit VulkanTexture(Device *device);
    };

    class VulkanTexture2D final: public Texture2D, public VulkanTexture
    {
    public:
        VulkanTexture2D(Device *device, Texture2DData *data, bool generateMipmaps);

    protected:
        void rebuild() override final;
        void rebuildSampler();
    };

    class VulkanCubeTexture final: public CubeTexture, public VulkanTexture
    {
    public:
        VulkanCubeTexture(Device *device, CubeTextureData *data);

    protected:
        void rebuild() override final;
        void rebuildSampler();
    };
}

#endif