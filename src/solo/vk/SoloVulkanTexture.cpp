/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVulkanTexture.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"
#include "SoloTextureData.h"

using namespace solo;

static auto toFilter(TextureFilter filter) -> VkFilter
{
    switch (filter)
    {
        case TextureFilter::Linear: return VK_FILTER_LINEAR;
        case TextureFilter::Nearest: return VK_FILTER_NEAREST;
    }

    SL_DEBUG_PANIC(true, "Unsupported texture filter");
    return VK_FILTER_BEGIN_RANGE;
}

static auto toMipmapMode(TextureMipFilter mipFilter) -> VkSamplerMipmapMode
{
    switch (mipFilter)
    {
        case TextureMipFilter::Linear: return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        case TextureMipFilter::Nearest: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case TextureMipFilter::None: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    }

    SL_DEBUG_PANIC(true, "Unsupported mip filter");
    return VK_SAMPLER_MIPMAP_MODE_END_RANGE;
}

static auto toAddressMode(TextureWrap wrap) -> VkSamplerAddressMode
{
    switch (wrap)
    {
        case TextureWrap::ClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case TextureWrap::ClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        case TextureWrap::MirrorRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case TextureWrap::Repeat: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    }

    SL_DEBUG_PANIC(true, "Unsupported wrap mode");
    return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
}

static auto createSampler(
    VkDevice device, VkPhysicalDeviceFeatures physicalFeatures, VkPhysicalDeviceProperties physicalProps,
    TextureFilter minFilter, TextureFilter magFilter, TextureMipFilter mipFilter, u32 mipLevels,
    TextureWrap horizontalWrap, TextureWrap verticalWrap, TextureWrap depthWrap,
    bool anisotropic, float anisotropyLevel) -> VulkanResource<VkSampler>
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.flags = 0;
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.maxAnisotropy = physicalFeatures.samplerAnisotropy
        ? (std::min)(anisotropyLevel, physicalProps.limits.maxSamplerAnisotropy)
        : 1;
    samplerInfo.anisotropyEnable = physicalFeatures.samplerAnisotropy && anisotropic;
    samplerInfo.magFilter = toFilter(magFilter);
    samplerInfo.minFilter = toFilter(minFilter);
    samplerInfo.mipmapMode = toMipmapMode(mipFilter);
    samplerInfo.addressModeU = toAddressMode(horizontalWrap);
    samplerInfo.addressModeV = toAddressMode(verticalWrap);
    samplerInfo.addressModeW = toAddressMode(depthWrap);
    samplerInfo.mipLodBias = 0;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerInfo.minLod = 0;
    samplerInfo.maxLod = static_cast<float>(mipFilter != TextureMipFilter::None ? mipLevels : 0);
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;

    VulkanResource<VkSampler> sampler{device, vkDestroySampler};
    SL_VK_CHECK_RESULT(vkCreateSampler(device, &samplerInfo, nullptr, sampler.cleanRef()));

    return sampler;
}

VulkanTexture::VulkanTexture(Device *device):
    renderer_(static_cast<VulkanRenderer*>(device->renderer()))
{
}

auto VulkanTexture2D::fromData(Device *device, sptr<Texture2DData> data, bool generateMipmaps) -> sptr<VulkanTexture2D>
{
    auto result = sptr<VulkanTexture2D>(new VulkanTexture2D(device, data->textureFormat(), data->dimensions()));
    result->image_ = VulkanImage::fromData(result->renderer_, data.get(), generateMipmaps);
    result->rebuildSampler();
    return result;
}

auto VulkanTexture2D::empty(Device *device, u32 width, u32 height, TextureFormat format) -> sptr<VulkanTexture2D>
{
    auto result = sptr<VulkanTexture2D>(new VulkanTexture2D(device, format,
        Vector2(static_cast<float>(width), static_cast<float>(height))));
    result->image_ = VulkanImage::empty(result->renderer_, width, height, format);
    result->rebuildSampler();
    return result;
}

VulkanTexture2D::VulkanTexture2D(Device *device, TextureFormat format, Vector2 dimensions):
    Texture2D(format, dimensions),
    VulkanTexture(device)
{
}

void VulkanTexture2D::rebuild()
{
    Texture2D::rebuild();
    rebuildSampler();
}

void VulkanTexture2D::rebuildSampler()
{
    sampler_ = createSampler(
        renderer_->device(),
        renderer_->physicalFeatures(),
        renderer_->physicalProperties(),
        minFilter_, magFilter_, mipFilter_, image_.mipLevels(),
        horizontalWrap_, verticalWrap_, TextureWrap::Repeat,
        anisotropyLevel_ > 1, anisotropyLevel_);
}

auto VulkanCubeTexture::fromData(Device *device, sptr<CubeTextureData> data) -> sptr<VulkanCubeTexture>
{
    auto result = sptr<VulkanCubeTexture>(new VulkanCubeTexture(device, data->textureFormat(), data->dimension()));
    result->image_ = VulkanImage::fromDataCube(result->renderer_, data.get());
    result->rebuildSampler();
    return result;
}

VulkanCubeTexture::VulkanCubeTexture(Device *device, TextureFormat format, u32 dimension):
    CubeTexture(format, dimension),
    VulkanTexture(device)
{
}

void VulkanCubeTexture::rebuild()
{
    CubeTexture::rebuild();
    rebuildSampler();
}

void VulkanCubeTexture::rebuildSampler()
{
    sampler_ = createSampler(
        renderer_->device(),
        renderer_->physicalFeatures(),
        renderer_->physicalProperties(),
        minFilter_, magFilter_, mipFilter_, image_.mipLevels(),
        horizontalWrap_, verticalWrap_, depthWrap_,
        anisotropyLevel_ > 1, anisotropyLevel_);
}

#endif
