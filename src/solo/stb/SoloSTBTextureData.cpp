/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloSTBTextureData.h"
#include "SoloStringUtils.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace solo;

static auto toFormat(int components) -> TextureDataFormat
{
    switch (components)
    {
        case 1: return TextureDataFormat::Red;
        case 3: return TextureDataFormat::RGB;
        case 4: return TextureDataFormat::RGBA;
        default:
            break;
    }

    SL_DEBUG_PANIC(true, "Components count ", components, " not converible to texture data format");
    return TextureDataFormat::RGB;
}

bool STBTexture2DData::canLoadFromFile(const str &path)
{
    static vec<str> supportedFormats = {".bmp", ".jpg", ".jpeg", ".png"};
    return std::find_if(supportedFormats.begin(), supportedFormats.end(),
        [&](const str &ext) { return stringutils::endsWith(path, ext); }) != supportedFormats.end();
}

STBTexture2DData::~STBTexture2DData()
{
    if (data_)
        stbi_image_free(data_);
}

auto STBTexture2DData::fromFile(Device *device, const str &path) -> sptr<STBTexture2DData>
{
    auto bytes = device->fileSystem()->readBytes(path);
    int width, height, channels;
    stbi_set_flip_vertically_on_load(device->mode() == DeviceMode::OpenGL);
    // According to the docs, channels are not affected by the requested channels
    const auto data = stbi_load_from_memory(bytes.data(), bytes.size(), &width, &height, &channels, 4);
    SL_DEBUG_PANIC(!data, "Unable to load image ", path);

    const auto result = std::make_shared<STBTexture2DData>(toFormat(4), Vector2(width, height));
    result->channels_ = 4;
    result->data_ = data;
    return result;
}

STBTexture2DData::STBTexture2DData(TextureDataFormat format, Vector2 dimensions):
    Texture2DData(format, dimensions)
{
}

bool STBCubeTextureData::canLoadFromFaceFiles(
    const str& positiveXPath, const str& negativeXPath,
    const str& positiveYPath, const str& negativeYPath,
    const str& positiveZPath, const str& negativeZPath)
{
    return STBTexture2DData::canLoadFromFile(positiveXPath) &&
        STBTexture2DData::canLoadFromFile(negativeXPath) &&
        STBTexture2DData::canLoadFromFile(positiveYPath) &&
        STBTexture2DData::canLoadFromFile(negativeYPath) &&
        STBTexture2DData::canLoadFromFile(positiveZPath) &&
        STBTexture2DData::canLoadFromFile(negativeZPath);
}

auto STBCubeTextureData::fromFaceFiles(
    Device *device,
    const str& positiveXPath, const str& negativeXPath,
    const str& positiveYPath, const str& negativeYPath,
    const str& positiveZPath, const str& negativeZPath) -> sptr<STBCubeTextureData>
{
    vec<sptr<STBTexture2DData>> faces;
    faces.push_back(STBTexture2DData::fromFile(device, positiveXPath));
    faces.push_back(STBTexture2DData::fromFile(device, negativeXPath));
    faces.push_back(STBTexture2DData::fromFile(device, positiveYPath));
    faces.push_back(STBTexture2DData::fromFile(device, negativeYPath));
    faces.push_back(STBTexture2DData::fromFile(device, positiveZPath));
    faces.push_back(STBTexture2DData::fromFile(device, negativeZPath));

    SL_DEBUG_BLOCK(
    {
        const auto dim = faces[0]->dimensions();
        SL_DEBUG_PANIC(dim.x() != dim.y(), "Cube texture width must be equal to height");
        for (const auto &face: faces)
            SL_DEBUG_PANIC(face->dimensions() != dim, "All cube texture sizes must match");
    });

    auto tex = std::make_shared<STBCubeTextureData>(faces[0]->format(), static_cast<u32>(faces[0]->dimensions().x()));
    tex->faces_ = std::move(faces);
    return tex;
}

STBCubeTextureData::STBCubeTextureData(TextureDataFormat format, u32 dimension):
    CubeTextureData(format, dimension)
{
}

auto STBCubeTextureData::size() const -> u32
{
    return faces_[0]->size() + 
           faces_[1]->size() + 
           faces_[2]->size() + 
           faces_[3]->size() + 
           faces_[4]->size() + 
           faces_[5]->size();
}

auto STBCubeTextureData::faceSize(u32 face) const -> u32
{
    return faces_[face]->size();
}

auto STBCubeTextureData::faceData(u32 face) const -> const void*
{
    return faces_[face]->data();
}
