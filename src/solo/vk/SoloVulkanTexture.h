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
		static auto createFromData(Device *device, Texture2DData *data, bool generateMipmaps) -> sptr<VulkanTexture2D>;

    protected:
		VulkanTexture2D(Device *device, TextureFormat format, Vector2 dimensions);
        void rebuild() override final;
        void rebuildSampler();
    };

    class VulkanCubeTexture final: public CubeTexture, public VulkanTexture
    {
    public:
        static auto createFromData(Device *device, CubeTextureData *data) -> sptr<VulkanCubeTexture>;

    protected:
		VulkanCubeTexture(Device *device, TextureFormat format, u32 dimension);
        void rebuild() override final;
        void rebuildSampler();
    };
}

#endif