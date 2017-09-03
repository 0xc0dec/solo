/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloRectTexture.h"
#include "SoloDevice.h"
#include "SoloTextureData.h"
#include "gl/SoloOpenGLRectTexture.h"
#include "null/SoloNullRectTexture.h"

using namespace solo;

auto RectTexture::loadFromFile(Device *device, const std::string &path) -> sptr<RectTexture>
{
    const auto data = RectTextureData::loadFromFile(device, path);
    auto tex = create(device, data->getWidth(0), data->getHeight(0), data->getFormat());
    tex->setData(data->getData());
    return tex;
}

sptr<RectTexture> RectTexture::create(Device *device, uint32_t width, uint32_t height, TextureFormat format)
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::RectTexture>(width, height, format);
#endif
        default:
            return std::make_shared<null::RectTexture>(width, height, format);
    }
}

RectTexture::RectTexture(uint32_t width, uint32_t height, TextureFormat format):
    format(format),
    dimensions{static_cast<float>(width), static_cast<float>(height)}
{
}
