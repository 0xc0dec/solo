/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloCubeTexture.h"
#include "SoloDevice.h"
#include "SoloTextureData.h"
#include "gl/SoloOpenGLCubeTexture.h"
#include "null/SoloNullCubeTexture.h"

using namespace solo;

auto CubeTexture::loadFromFaceFiles(Device *device,
    const std::string &frontPath,
    const std::string &backPath,
    const std::string &leftPath,
    const std::string &rightPath,
    const std::string &topPath,
    const std::string &bottomPath) -> sptr<CubeTexture>
{
    auto data = CubeTextureData::loadFromFaceFiles(device, frontPath, backPath, leftPath, rightPath, topPath, bottomPath);
    auto tex = create(device, data->getWidth(0), data->getHeight(0), 6, data->getFormat());
    tex->setData(data.get());
    return tex;
}

auto CubeTexture::create(Device *device, uint32_t width, uint32_t height, uint32_t depth, TextureFormat format) -> sptr<CubeTexture>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::CubeTexture>(width, height, depth, format);
#endif
        default:
            return std::make_shared<null::CubeTexture>(width, height, depth, format);
    }
}

CubeTexture::CubeTexture(uint32_t width, uint32_t height, uint32_t depth, TextureFormat format):
    width(width),
    height(height),
    depth(depth),
    format(format)
{
    rebuildFlags();
}

void CubeTexture::setWrapping(TextureWrapping wrap)
{
    verticalWrapping = wrap;
    horizontalWrapping = wrap;
    depthWrapping = wrap;
    rebuildFlags();
}

void CubeTexture::rebuildFlags()
{
    Texture::rebuildFlags();

    switch (depthWrapping)
    {
        case TextureWrapping::Clamp:
            flags |= TextureFlags::DepthWrapClamp;
            break;
        case TextureWrapping::Repeat:
            flags |= TextureFlags::DepthWrapRepeat;
            break;
        default:
            break;
    }
}
