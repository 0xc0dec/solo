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

void Texture::setWrap(TextureWrap wrap)
{
    horizontalWrap = wrap;
    verticalWrap = wrap;
    rebuild();
}

void Texture::setVerticalWrap(TextureWrap wrap)
{
    verticalWrap = wrap;
    rebuild();
}

void Texture::setHorizontalWrap(TextureWrap wrap)
{
    horizontalWrap = wrap;
    rebuild();
}

void Texture::setFilter(TextureFilter minFiltering, TextureFilter magFiltering, TextureMipFilter mipFiltering)
{
    this->minFilter = minFiltering;
    this->magFilter = magFiltering;
    this->mipFilter = mipFiltering;
    rebuild();
}

void Texture::setAnisotropyLevel(float level)
{
    anisotropyLevel = level;
    rebuild();
}

Texture::Texture(TextureFormat format):
    format(format)
{
    rebuild(); // yes, virtual call
}

auto Texture2D::loadFromFile(Device *device, const str &path, bool generateMipmaps) -> sptr<Texture2D>
{
    const auto data = Texture2DData::loadFromFile(device, path);
    return createFromData(device, data, generateMipmaps);
}

auto Texture2D::loadFromFileAsync(Device *device, const str &path, bool generateMipmaps) -> sptr<AsyncHandle<Texture2D>>
{
    auto handle = std::make_shared<AsyncHandle<Texture2D>>();

    auto producers = JobBase<Texture2DData>::Producers{[=]() { return Texture2DData::loadFromFile(device, path); }};
    auto consumer = [handle, device, generateMipmaps](const vec<sptr<Texture2DData>> &results)
    {
        auto texture = createFromData(device, results[0], generateMipmaps);
        handle->resolve(texture);
    };

    device->getJobPool()->addJob(std::make_shared<JobBase<Texture2DData>>(producers, consumer));

    return handle;
}

auto Texture2D::createEmpty(Device *device, u32 width, u32 height, TextureFormat format) -> sptr<Texture2D>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return OpenGLTexture2D::createEmpty(width, height, format);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanTexture2D::createEmpty(device, width, height, format);
#endif
        default:
            return std::make_shared<NullTexture2D>(format, Vector2(width, height));
    }
}

auto Texture2D::createFromData(Device *device, sptr<Texture2DData> data, bool generateMipmaps) -> sptr<Texture2D>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return OpenGLTexture2D::createFromData(data, generateMipmaps);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanTexture2D::createFromData(device, data, generateMipmaps);
#endif
        default:
            return std::make_shared<NullTexture2D>(data->getTextureFormat(), data->getDimensions());
    }
}

Texture2D::Texture2D(TextureFormat format, Vector2 dimensions):
    Texture(format),
    dimensions(dimensions)
{
}

auto CubeTexture::loadFromFaceFiles(
    Device *device,
    const str& positiveXPath, const str& negativeXPath,
    const str& positiveYPath, const str& negativeYPath,
    const str& positiveZPath, const str& negativeZPath) -> sptr<CubeTexture>
{
    const auto data = CubeTextureData::loadFromFaceFiles(
        device,
        positiveXPath, negativeXPath,
        positiveYPath, negativeYPath,
        positiveZPath, negativeZPath);
    return createFromData(device, data);
}

auto CubeTexture::loadFromFaceFilesAsync(
    Device *device,
    const str& positiveXPath, const str& negativeXPath,
    const str& positiveYPath, const str& negativeYPath,
    const str& positiveZPath, const str& negativeZPath) -> sptr<AsyncHandle<CubeTexture>>
{
    auto handle = std::make_shared<AsyncHandle<CubeTexture>>();

    auto producers = JobBase<CubeTextureData>::Producers{[=]()
    {
        // TODO run each face loading in separate jobs
        return CubeTextureData::loadFromFaceFiles(
            device,
            positiveXPath, negativeXPath,
            positiveYPath, negativeYPath,
            positiveZPath, negativeZPath);
    }};
    auto consumer = [handle, device](const vec<sptr<CubeTextureData>> &results)
    {
        auto texture = createFromData(device, results[0]);
        handle->resolve(texture);
    };

    device->getJobPool()->addJob(std::make_shared<JobBase<CubeTextureData>>(producers, consumer));

    return handle;
}

auto CubeTexture::createFromData(Device *device, sptr<CubeTextureData> data) -> sptr<CubeTexture>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return OpenGLCubeTexture::createFromData(data);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanCubeTexture::createFromData(device, data);
#endif
        default:
            return std::make_shared<NullCubeTexture>(data->getTextureFormat(), data->getDimension());
    }
}

CubeTexture::CubeTexture(TextureFormat format, u32 dimension):
    Texture(format),
    dimension(dimension)
{
    rebuild();
}

void CubeTexture::setWrap(TextureWrap wrap)
{
    verticalWrap = wrap;
    horizontalWrap = wrap;
    depthWrap = wrap;
    rebuild();
}

void CubeTexture::setDepthWrap(TextureWrap wrap)
{
    this->depthWrap = wrap;
    rebuild();
}

