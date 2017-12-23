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

static auto createSampler(
    VkDevice device, VkPhysicalDeviceFeatures physicalFeatures, VkPhysicalDeviceProperties physicalProps,
    bool anisotropic, float anisotropyLevel, u32 flags
) -> VulkanResource<VkSampler>
{
    // TODO Better support for flags

    auto minFilter = VK_FILTER_NEAREST;
    if (flags & TextureFlags::MinFilterLinear ||
        flags & TextureFlags::MinFilterLinearMipmapLinear ||
        flags & TextureFlags::MinFilterLinearMipmapNearest)
        minFilter = VK_FILTER_LINEAR;

    auto magFilter = VK_FILTER_NEAREST;
    if (flags & TextureFlags::MagFilterLinear ||
        flags & TextureFlags::MagFilterLinearMipmapLinear ||
        flags & TextureFlags::MagFilterLinearMipmapNearest)
        magFilter = VK_FILTER_LINEAR;

    auto addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    if (flags & TextureFlags::HorizontalWrapClamp)
        addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

    auto addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    if (flags & TextureFlags::VerticalWrapClamp)
        addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

    auto addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    if (flags & TextureFlags::DepthWrapClamp)
        addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.flags = 0;
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.maxAnisotropy = (std::min)(anisotropyLevel, physicalProps.limits.maxSamplerAnisotropy);
    samplerInfo.anisotropyEnable = physicalFeatures.samplerAnisotropy && anisotropic;
    samplerInfo.magFilter = magFilter;
    samplerInfo.minFilter = minFilter;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = addressModeU;
    samplerInfo.addressModeV = addressModeV;
    samplerInfo.addressModeW = addressModeW;
    samplerInfo.mipLodBias = 0;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.minLod = 0;
    samplerInfo.maxLod = 0;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;

    VulkanResource<VkSampler> sampler{device, vkDestroySampler};
    SL_VK_CHECK_RESULT(vkCreateSampler(device, &samplerInfo, nullptr, sampler.cleanRef()));

    return sampler;
}

VulkanTexture::VulkanTexture(Device *device):
    renderer(static_cast<VulkanRenderer*>(device->getRenderer()))
{
}

void VulkanTexture::rebuildSampler(float anisotropyLevel, u32 flags)
{
    sampler = createSampler(
        renderer->getDevice(),
        renderer->getPhysicalFeatures(), renderer->getPhysicalProperties(),
        anisotropyLevel > 1, anisotropyLevel, flags); // TODO Better calculation of anisotropy-enabling flag
}

VulkanTexture2d::VulkanTexture2d(Device *device, Texture2dData *data):
    Texture2d(data),
    VulkanTexture(device)
{
    const auto renderer = static_cast<VulkanRenderer*>(device->getRenderer());
    image = VulkanImage::create2d(renderer, data);
    rebuildSampler(anisotropyLevel, flags);
}

void VulkanTexture2d::generateMipmaps()
{
    // TODO
}

void VulkanTexture2d::rebuild()
{
    Texture2d::rebuild();
    rebuildSampler(anisotropyLevel, flags);
}

VulkanCubeTexture::VulkanCubeTexture(Device *device, CubeTextureData *data):
    CubeTexture(data),
    VulkanTexture(device)
{
    const auto renderer = static_cast<VulkanRenderer*>(device->getRenderer());
    image = VulkanImage::createCube(renderer, data);
    rebuildSampler(anisotropyLevel, flags);
}

void VulkanCubeTexture::generateMipmaps()
{
    // TODO
}

void VulkanCubeTexture::rebuild()
{
    CubeTexture::rebuild();
    rebuildSampler(anisotropyLevel, flags);
}

#endif
