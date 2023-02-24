/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloTexture.h"
#include "SoloDevice.h"
#include "SoloTextureData.h"
#include "SoloJobPool.h"
#include "gl/SoloOpenGLTexture.h"
#include "vk/SoloVulkanTexture.h"

using namespace solo;

void Texture::setWrap(TextureWrap wrap) {
    horizontalWrap_ = wrap;
    verticalWrap_ = wrap;
    rebuild();
}

void Texture::setVerticalWrap(TextureWrap wrap) {
    verticalWrap_ = wrap;
    rebuild();
}

void Texture::setHorizontalWrap(TextureWrap wrap) {
    horizontalWrap_ = wrap;
    rebuild();
}

void Texture::setFilter(TextureFilter minFiltering, TextureFilter magFiltering, TextureMipFilter mipFiltering) {
    this->minFilter_ = minFiltering;
    this->magFilter_ = magFiltering;
    this->mipFilter_ = mipFiltering;
    rebuild();
}

void Texture::setAnisotropyLevel(float level) {
    anisotropyLevel_ = level;
    rebuild();
}

Texture::Texture(TextureFormat format):
    format_(format) {
    rebuild(); // yes, virtual call
}

auto Texture2D::fromFile(Device *device, const str &path, bool generateMipmaps) -> sptr<Texture2D> {
    const auto data = Texture2DData::fromFile(device, path);
    return fromData(device, data, generateMipmaps);
}

auto Texture2D::fromFileAsync(Device *device, const str &path, bool generateMipmaps) -> sptr<AsyncHandle<Texture2D>> {
    auto handle = std::make_shared<AsyncHandle<Texture2D>>();

    auto producers = JobBase<Texture2DData>::Producers{
        [ = ]() {
            return Texture2DData::fromFile(device, path);
        }};
    auto consumer = [handle, device, generateMipmaps](const vec<sptr<Texture2DData>> &results) {
        const auto texture = fromData(device, results[0], generateMipmaps);
        handle->resolve(texture);
    };

    device->jobPool()->addJob(std::make_shared<JobBase<Texture2DData>>(producers, consumer));

    return handle;
}

auto Texture2D::empty(Device *device, u32 width, u32 height, TextureFormat format) -> sptr<Texture2D> {
    switch (device->mode()) {
#ifdef SL_OPENGL_RENDERER
    case DeviceMode::OpenGL:
        return OpenGLTexture2D::empty(width, height, format);
#endif
#ifdef SL_VULKAN_RENDERER
    case DeviceMode::Vulkan:
        return VulkanTexture2D::empty(device, width, height, format);
#endif
    default:
        panic("Unknown device mode");
        return nullptr;
    }
}

auto Texture2D::fromData(Device *device, sptr<Texture2DData> data, bool generateMipmaps) -> sptr<Texture2D> {
    switch (device->mode()) {
#ifdef SL_OPENGL_RENDERER
    case DeviceMode::OpenGL:
        return OpenGLTexture2D::fromData(data, generateMipmaps);
#endif
#ifdef SL_VULKAN_RENDERER
    case DeviceMode::Vulkan:
        return VulkanTexture2D::fromData(device, data, generateMipmaps);
#endif
    default:
        panic("Unknown device mode");
        return nullptr;
    }
}

Texture2D::Texture2D(TextureFormat format, Vector2 dimensions):
    Texture(format),
    dimensions_(dimensions) {
}

auto CubeTexture::fromFaceFiles(
    Device *device,
    const str &positiveXPath, const str &negativeXPath,
    const str &positiveYPath, const str &negativeYPath,
    const str &positiveZPath, const str &negativeZPath) -> sptr<CubeTexture> {
    const auto data = CubeTextureData::fromFaceFiles(
        device,
        positiveXPath, negativeXPath,
        positiveYPath, negativeYPath,
        positiveZPath, negativeZPath);
    return fromData(device, data);
}

auto CubeTexture::fromFaceFilesAsync(
    Device *device,
    const str &positiveXPath, const str &negativeXPath,
    const str &positiveYPath, const str &negativeYPath,
const str &positiveZPath, const str &negativeZPath) -> sptr<AsyncHandle<CubeTexture>> {
    auto handle = std::make_shared<AsyncHandle<CubeTexture>>();

    auto producers = JobBase<CubeTextureData>::Producers{
        [ = ]() {
            // TODO run each face loading in separate jobs
            return CubeTextureData::fromFaceFiles(
                device,
                positiveXPath, negativeXPath,
                positiveYPath, negativeYPath,
                positiveZPath, negativeZPath);
        }};
    auto consumer = [handle, device](const vec<sptr<CubeTextureData>> &results) {
        const auto texture = fromData(device, results[0]);
        handle->resolve(texture);
    };

    device->jobPool()->addJob(std::make_shared<JobBase<CubeTextureData>>(producers, consumer));

    return handle;
}

auto CubeTexture::fromData(Device *device, sptr<CubeTextureData> data) -> sptr<CubeTexture> {
    switch (device->mode()) {
#ifdef SL_OPENGL_RENDERER
    case DeviceMode::OpenGL:
        return OpenGLCubeTexture::fromData(data);
#endif
#ifdef SL_VULKAN_RENDERER
    case DeviceMode::Vulkan:
        return VulkanCubeTexture::fromData(device, data);
#endif
    default:
        panic("Unknown device mode");
        return nullptr;
    }
}

CubeTexture::CubeTexture(TextureFormat format, u32 dimension):
    Texture(format),
    dimension_(dimension) {
    rebuild(); // yes, virtual call
}

void CubeTexture::setWrap(TextureWrap wrap) {
    verticalWrap_ = wrap;
    horizontalWrap_ = wrap;
    depthWrap_ = wrap;
    rebuild();
}

void CubeTexture::setDepthWrap(TextureWrap wrap) {
    this->depthWrap_ = wrap;
    rebuild();
}

