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

static auto convertTextureFormat(TextureFormat format) -> VkFormat
{
    switch (format)
    {
        case TextureFormat::RGBA: return VK_FORMAT_R8G8B8A8_UNORM;
        case TextureFormat::Red: return VK_FORMAT_R8_UNORM;
        default:
            SL_PANIC(SL_FMT("Unsupported texture format ", static_cast<u32>(format)));
            return VK_FORMAT_UNDEFINED;
    }
}

vk::VulkanTexture2d::VulkanTexture2d(Device *device, Texture2dData *data):
    solo::Texture2d(data)
{
    const auto renderer = static_cast<vk::VulkanRenderer*>(device->getRenderer());
    image = VulkanImage::create2d(renderer, data);
}

void vk::VulkanTexture2d::generateMipmaps()
{
    // TODO
}

#endif
