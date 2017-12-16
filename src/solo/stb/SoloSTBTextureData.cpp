/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSTBTextureData.h"
#include "SoloStringUtils.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace solo;

static auto toImageFormat(int components) -> TextureFormat
{
    switch (components)
    {
        case 1: return TextureFormat::Red; // TODO Not sure about this
        case 3: return TextureFormat::RGB;
        case 4: return TextureFormat::RGBA;
        default:
            SL_PANIC(SL_FMT("Unsupported components count ", components))
            return TextureFormat::RGB;
    }
}

bool STBTexture2dData::canLoadFromFile(const str &path)
{
    static vec<str> supportedFormats = {".bmp", ".jpg", ".jpeg", ".png"};
    return std::find_if(supportedFormats.begin(), supportedFormats.end(),
        [&](const str &ext) { return stringutils::endsWith(path, ext); }) != supportedFormats.end();
}

STBTexture2dData::~STBTexture2dData()
{
    if (data)
        stbi_image_free(data);
}

auto STBTexture2dData::loadFromFile(Device *device, const str &path) -> sptr<STBTexture2dData>
{
    auto bytes = device->getFileSystem()->readBytes(path);
    int width, height, bpp;
    stbi_set_flip_vertically_on_load(device->getMode() == DeviceMode::OpenGL);
    const auto data = stbi_load_from_memory(bytes.data(), bytes.size(), &width, &height, &bpp, 0);
    SL_PANIC_IF(!data, SL_FMT("Failed to load image ", path));

    const auto result = std::make_shared<STBTexture2dData>();
    result->bpp = bpp;
    result->format = toImageFormat(bpp);
    result->width = width;
    result->height = height;
    result->data = data;
    return result;
}

bool STBCubeTextureData::canLoadFromFaceFiles(
    const str &frontPath,
    const str &backPath,
    const str &leftPath,
    const str &rightPath,
    const str &topPath,
    const str &bottomPath)
{
    return STBTexture2dData::canLoadFromFile(frontPath) &&
           STBTexture2dData::canLoadFromFile(backPath) &&
           STBTexture2dData::canLoadFromFile(leftPath) &&
           STBTexture2dData::canLoadFromFile(rightPath) &&
           STBTexture2dData::canLoadFromFile(topPath) &&
           STBTexture2dData::canLoadFromFile(bottomPath);
}

auto STBCubeTextureData::loadFromFaceFiles(
    Device *device,
    const str &frontPath,
    const str &backPath,
    const str &leftPath,
    const str &rightPath,
    const str &topPath,
    const str &bottomPath) -> sptr<STBCubeTextureData>
{
    auto tex = std::make_shared<STBCubeTextureData>();
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, frontPath));
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, backPath));
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, leftPath));
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, rightPath));
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, topPath));
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, bottomPath));
    
    SL_PANIC_BLOCK(
    {
        auto width = tex->faces[0]->getWidth();
        auto height = tex->faces[0]->getHeight();
        SL_PANIC_IF(width != height, "Cube texture width must be equal to height");
        for (const auto &face: tex->faces)
            SL_PANIC_IF(face->getWidth() != width || face->getHeight() != height, "All cube texture sizes must match");
    });

    return tex;
}

auto STBCubeTextureData::getSize() const -> u32
{
    return faces[0]->getSize() + 
           faces[1]->getSize() + 
           faces[2]->getSize() + 
           faces[3]->getSize() + 
           faces[4]->getSize() + 
           faces[5]->getSize();
}

auto STBCubeTextureData::getSize(u32 face) const -> u32
{
    return faces[face]->getSize();
}

auto STBCubeTextureData::getDimension() const -> u32
{
    return faces[0]->getWidth();
}

auto STBCubeTextureData::getData() const -> const void*
{
    SL_PANIC("Not implemented");
    return nullptr;
}

auto STBCubeTextureData::getData(u32 face) const -> const void*
{
    return faces[face]->getData();
}

auto STBCubeTextureData::getFormat() const -> TextureFormat
{
    return faces[0]->getFormat();
}