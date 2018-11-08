/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
        auto image() const -> VulkanImage const& { return image_; }
        auto sampler() const -> VkSampler { return sampler_; }

    protected:
        VulkanRenderer *renderer_ = nullptr;
        VulkanImage image_;
        VulkanResource<VkSampler> sampler_;

        explicit VulkanTexture(Device *device);
    };

    class VulkanTexture2D final: public Texture2D, public VulkanTexture
    {
    public:
        static auto fromData(Device *device, sptr<Texture2DData> data, bool generateMipmaps) -> sptr<VulkanTexture2D>;
        static auto empty(Device *device, u32 width, u32 height, TextureFormat format) -> sptr<VulkanTexture2D>;

    protected:
        VulkanTexture2D(Device *device, TextureFormat format, Vector2 dimensions);
        void rebuild() override final;
        void rebuildSampler();
    };

    class VulkanCubeTexture final: public CubeTexture, public VulkanTexture
    {
    public:
        static auto fromData(Device *device, sptr<CubeTextureData> data) -> sptr<VulkanCubeTexture>;

    protected:
        VulkanCubeTexture(Device *device, TextureFormat format, u32 dimension);
        void rebuild() override final;
        void rebuildSampler();
    };
}

#endif