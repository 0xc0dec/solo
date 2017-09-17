/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTexture.h"
#include "SoloDevice.h"
#include "SoloTextureData.h"
#include "SoloJobPool.h"
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
    return createFromData(device, data.get());
}

auto Texture2d::loadFromFileAsync(Device *device, const std::string &path) -> sptr<AsyncHandle<Texture2d>>
{
    auto handle = std::make_shared<AsyncHandle<Texture2d>>();

    auto producers = JobBase<Texture2dData>::Producers{[=]() { return Texture2dData::loadFromFile(device, path); }};
    auto consumer = [handle, device](const std::vector<sptr<Texture2dData>> &results)
    {
        auto texture = createFromData(device, results[0].get());
        handle->finish(texture);
    };

    device->getJobPool()->add(std::make_shared<JobBase<Texture2dData>>(producers, consumer));

    return handle;
}

auto Texture2d::createEmpty(Device *device, uint32_t width, uint32_t height, TextureFormat format) -> sptr<Texture2d>
{
    const auto data = Texture2dData::createFromMemory(width, height, format, std::vector<uint8_t>{});
    return createFromData(device, data.get());
}

sptr<Texture2d> Texture2d::createFromData(Device *device, Texture2dData *data)
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Texture2d>(data);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<vk::Texture2d>(device, data);
#endif
        default:
            return std::make_shared<null::Texture2d>(data);
    }
}

Texture2d::Texture2d(Texture2dData *data):
    format(data->getFormat()),
    dimensions{static_cast<float>(data->getWidth()), static_cast<float>(data->getHeight())}
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
    return create(device, data.get());
}

auto CubeTexture::loadFromFaceFilesAsync(Device *device,
    const std::string &frontPath,
    const std::string &backPath,
    const std::string &leftPath,
    const std::string &rightPath,
    const std::string &topPath,
    const std::string &bottomPath) -> sptr<AsyncHandle<CubeTexture>>
{
    auto handle = std::make_shared<AsyncHandle<CubeTexture>>();

    auto producers = JobBase<CubeTextureData>::Producers{[=]()
    {
        return CubeTextureData::loadFromFaceFiles(device,
            frontPath, backPath,
            leftPath, rightPath,
            topPath, bottomPath);
    }};
    auto consumer = [handle, device](const std::vector<sptr<CubeTextureData>> &results)
    {
        auto texture = create(device, results[0].get());
        handle->finish(texture);
    };

    device->getJobPool()->add(std::make_shared<JobBase<CubeTextureData>>(producers, consumer));

    return handle;
}

auto CubeTexture::create(Device *device, CubeTextureData *data) -> sptr<CubeTexture>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::CubeTexture>(data);
#endif
        default:
            return std::make_shared<null::CubeTexture>(data);
    }
}

CubeTexture::CubeTexture(CubeTextureData *data):
    dimension(data->getDimension()),
    format(data->getFormat())
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
