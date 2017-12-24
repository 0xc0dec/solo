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

        void rebuildSampler(float anisotropyLevel, u32 flags);
    };

    class VulkanTexture2d final: public Texture2d, public VulkanTexture
    {
    public:
        VulkanTexture2d(Device *device, Texture2dData *data, bool generateMipmaps);

    protected:
        void rebuild() override final;
    };

    class VulkanCubeTexture final: public CubeTexture, public VulkanTexture
    {
    public:
        VulkanCubeTexture(Device *device, CubeTextureData *data);

    protected:
        void rebuild() override final;
    };
}

#endif