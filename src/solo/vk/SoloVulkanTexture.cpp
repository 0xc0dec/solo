/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
        default:
            return panic<VkFilter>("Unsupported filter");
    }
}

static auto toMipmapMode(TextureMipFilter mipFilter) -> VkSamplerMipmapMode
{
    switch (mipFilter)
    {
        case TextureMipFilter::Linear: return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        case TextureMipFilter::Nearest: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case TextureMipFilter::None: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        default:
            return panic<VkSamplerMipmapMode>("Unsupported mip filter");
    }
}

static auto toAddressMode(TextureWrap wrap) -> VkSamplerAddressMode
{
    switch (wrap)
    {
        case TextureWrap::ClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case TextureWrap::ClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        case TextureWrap::MirrorRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case TextureWrap::Repeat: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        default:
            return panic<VkSamplerAddressMode>("Unsupported wrap mode");
    }
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
    renderer(static_cast<VulkanRenderer*>(device->getRenderer()))
{
}

auto VulkanTexture2D::createFromData(Device *device, Texture2DData *data, bool generateMipmaps) -> sptr<VulkanTexture2D>
{
    auto result = sptr<VulkanTexture2D>(new VulkanTexture2D(device, data->getTextureFormat(), data->getDimensions()));
    result->image = VulkanImage::create2D(result->renderer, data, generateMipmaps);
    result->rebuildSampler();
    return result;
}

auto VulkanTexture2D::createEmpty(Device *device, u32 width, u32 height, TextureFormat format) -> sptr<VulkanTexture2D>
{
    auto result = sptr<VulkanTexture2D>(new VulkanTexture2D(device, format,
        Vector2(static_cast<float>(width), static_cast<float>(height))));
    result->image = VulkanImage::createEmpty2D(result->renderer, width, height, format);
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
    sampler = createSampler(
        renderer->getDevice(),
        renderer->getPhysicalFeatures(),
        renderer->getPhysicalProperties(),
        minFilter, magFilter, mipFilter, image.getMipLevels(),
        horizontalWrap, verticalWrap, TextureWrap::Repeat,
        anisotropyLevel > 1, anisotropyLevel);
}

auto VulkanCubeTexture::createFromData(Device *device, CubeTextureData *data) -> sptr<VulkanCubeTexture>
{
    auto result = sptr<VulkanCubeTexture>(new VulkanCubeTexture(device, data->getTextureFormat(), data->getDimension()));
    result->image = VulkanImage::createCube(result->renderer, data);
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
    sampler = createSampler(
        renderer->getDevice(),
        renderer->getPhysicalFeatures(),
        renderer->getPhysicalProperties(),
        minFilter, magFilter, mipFilter, image.getMipLevels(),
        horizontalWrap, verticalWrap, depthWrap,
        anisotropyLevel > 1, anisotropyLevel);
}

#endif
