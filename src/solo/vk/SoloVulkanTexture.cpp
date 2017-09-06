/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanTexture.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"

using namespace solo;

static auto convertTextureFormat(TextureFormat format) -> VkFormat
{
    switch (format)
    {
        case TextureFormat::RGBA: return VK_FORMAT_R8G8B8A8_UNORM;
        case TextureFormat::Red: return VK_FORMAT_R8_UNORM;
        default:
            SL_PANIC("Unsupported texture format ", SL_FMT(static_cast<uint32_t>(format)));
            return VK_FORMAT_UNDEFINED;
    }
}

vk::Texture2d::Texture2d(Device *device, uint32_t width, uint32_t height, TextureFormat format):
    solo::Texture2d(width, height, format)
{
    const auto renderer = static_cast<vk::Renderer*>(device->getRenderer());
    image = Image(renderer, width, height, 1, 1, convertTextureFormat(format),
        0,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_IMAGE_ASPECT_COLOR_BIT);
}

void vk::Texture2d::generateMipmaps()
{
    // TODO
}

void vk::Texture2d::setData(const void *data)
{
    // TODO
}

#endif
