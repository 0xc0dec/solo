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

VulkanTexture2d::VulkanTexture2d(Device *device, Texture2dData *data):
    Texture2d(data)
{
    const auto renderer = static_cast<VulkanRenderer*>(device->getRenderer());
    image = VulkanImage::create2d(renderer, data);
}

void VulkanTexture2d::generateMipmaps()
{
    // TODO
}


VulkanCubeTexture::VulkanCubeTexture(Device *device, CubeTextureData *data):
    CubeTexture(data)
{
    const auto renderer = static_cast<VulkanRenderer*>(device->getRenderer());
    image = VulkanImage::createCube(renderer, data);
}

void VulkanCubeTexture::generateMipmaps()
{
    // TODO
}

#endif
