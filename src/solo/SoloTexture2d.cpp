/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTexture2d.h"
#include "SoloDevice.h"
#include "SoloTextureData.h"
#include "gl/SoloOpenGLTexture2d.h"
#include "vk/SoloVulkanTexture.h"
#include "null/SoloNullTexture2d.h"

using namespace solo;

auto Texture2d::loadFromFile(Device *device, const std::string &path) -> sptr<Texture2d>
{
    const auto data = Texture2dData::loadFromFile(device, path);
    auto tex = create(device, data->getWidth(0), data->getHeight(0), data->getFormat());
    tex->setData(data->getData());
    return tex;
}

sptr<Texture2d> Texture2d::create(Device *device, uint32_t width, uint32_t height, TextureFormat format)
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Texture2d>(width, height, format);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<vk::Texture2d>(device, width, height, format);
#endif
        default:
            return std::make_shared<null::Texture2d>(width, height, format);
    }
}

Texture2d::Texture2d(uint32_t width, uint32_t height, TextureFormat format):
    format(format),
    dimensions{static_cast<float>(width), static_cast<float>(height)}
{
}
