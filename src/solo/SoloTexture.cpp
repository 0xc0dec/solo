/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTexture.h"
#include "SoloDevice.h"
#include "SoloTextureData.h"
#include "gl/SoloOpenGLTexture.h"
#include "vk/SoloVulkanTexture.h"
#include "null/SoloNullTexture.h"

using namespace solo;

Texture::Texture()
{
    rebuildFlags(); // yes, virtual call
}

void Texture::setWrapping(TextureWrapping wrap)
{
    horizontalWrapping = wrap;
    verticalWrapping = wrap;
    rebuildFlags();
}

void Texture::rebuildFlags()
{
    flags = 0;

    switch (horizontalWrapping)
    {
        case TextureWrapping::Clamp:
            flags |= TextureFlags::HorizontalWrapClamp;
            break;
        case TextureWrapping::Repeat:
            flags |= TextureFlags::HorizontalWrapRepeat;
            break;
        default:
            break;
    }

    switch (verticalWrapping)
    {
        case TextureWrapping::Clamp:
            flags |= TextureFlags::VerticalWrapClamp;
            break;
        case TextureWrapping::Repeat:
            flags |= TextureFlags::VerticalWrapRepeat;
            break;
        default:
            break;
    }

    switch (minFiltering)
    {
        case TextureFiltering::Nearest:
            flags |= TextureFlags::MinFilterNearest;
            break;
        case TextureFiltering::Linear:
            flags |= TextureFlags::MinFilterLinear;
            break;
        case TextureFiltering::NearestMipmapNearest:
            flags |= TextureFlags::MinFilterNearestMipmapNearest;
            break;
        case TextureFiltering::LinearMipmapNearest:
            flags |= TextureFlags::MinFilterLinearMipmapNearest;
            break;
        case TextureFiltering::NearestMipmapLinear:
            flags |= TextureFlags::MinFilterNearestMipmapLinear;
            break;
        case TextureFiltering::LinearMipmapLinear:
            flags |= TextureFlags::MinFilterLinearMipmapLinear;
            break;
        default:
            break;
    }

    switch (magFiltering)
    {
        case TextureFiltering::Nearest:
            flags |= TextureFlags::MagFilterNearest;
            break;
        case TextureFiltering::Linear:
            flags |= TextureFlags::MagFilterLinear;
            break;
        case TextureFiltering::NearestMipmapNearest:
            flags |= TextureFlags::MagFilterNearestMipmapNearest;
            break;
        case TextureFiltering::LinearMipmapNearest:
            flags |= TextureFlags::MagFilterLinearMipmapNearest;
            break;
        case TextureFiltering::NearestMipmapLinear:
            flags |= TextureFlags::MagFilterNearestMipmapLinear;
            break;
        case TextureFiltering::LinearMipmapLinear:
            flags |= TextureFlags::MagFilterLinearMipmapLinear;
            break;
        default:
            break;
    }
}

void Texture::setVerticalWrapping(TextureWrapping wrap)
{
    verticalWrapping = wrap;
    rebuildFlags();
}

void Texture::setHorizontalWrapping(TextureWrapping wrap)
{
    horizontalWrapping = wrap;
    rebuildFlags();
}

void Texture::setMinFiltering(TextureFiltering filtering)
{
    minFiltering = filtering;
    rebuildFlags();
}

void Texture::setMagFiltering(TextureFiltering filtering)
{
    magFiltering = filtering;
    rebuildFlags();
}

void Texture::setFiltering(TextureFiltering filtering)
{
    minFiltering = filtering;
    magFiltering = filtering;
    rebuildFlags();
}

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

auto CubeTexture::loadFromFaceFiles(Device *device,
    const std::string &frontPath,
    const std::string &backPath,
    const std::string &leftPath,
    const std::string &rightPath,
    const std::string &topPath,
    const std::string &bottomPath) -> sptr<CubeTexture>
{
    auto data = CubeTextureData::loadFromFaceFiles(device, frontPath, backPath, leftPath, rightPath, topPath, bottomPath);
    auto tex = create(device, data->getDimension(0), data->getFormat());
    tex->setData(data.get());
    return tex;
}

auto CubeTexture::create(Device *device, uint32_t dimension, TextureFormat format) -> sptr<CubeTexture>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::CubeTexture>(dimension, format);
#endif
        default:
            return std::make_shared<null::CubeTexture>(dimension, format);
    }
}

CubeTexture::CubeTexture(uint32_t dimension, TextureFormat format):
    dimension(dimension),
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
