/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTextureData.h"
#include "stb/SoloSTBTextureData.h"

namespace solo
{
    class InMemoryTexture2DData final: public Texture2DData
    {
    public:
        explicit InMemoryTexture2DData(Vector2 dimensions, TextureDataFormat format, const vec<u8> &data):
            Texture2DData(format, dimensions),
            data(data)
        {
        }

        auto getSize() const -> u32 override final { return data.size(); }
        auto getData() const -> const void* override final { return data.data(); }

    private:
        vec<u8> data;
    };
}

using namespace solo;

static auto toTextureFormat(TextureDataFormat format) -> TextureFormat
{
    switch (format)
    {
        case TextureDataFormat::Red:
            return TextureFormat::Red;
        case TextureDataFormat::RGB:
            return TextureFormat::RGB;
        case TextureDataFormat::RGBA:
            return TextureFormat::RGBA;
        default:
            return panic<TextureFormat>("Texture data format not convertible to texture format");
    }
}

auto Texture2DData::loadFromFile(Device *device, const str &path) -> sptr<Texture2DData>
{
    if (STBTexture2DData::canLoadFromFile(path))
        return STBTexture2DData::loadFromFile(device, path);
    return panic<nullptr_t>(SL_FMT("Unsupported cube texture file ", path));
}

auto Texture2DData::createFromMemory(u32 width, u32 height, TextureDataFormat format, const vec<u8> &data) -> sptr<Texture2DData>
{
    return std::make_shared<InMemoryTexture2DData>(Vector2(width, height), format, data);
}

auto Texture2DData::getTextureFormat() const -> TextureFormat
{
    return toTextureFormat(format);
}

Texture2DData::Texture2DData(TextureDataFormat format, Vector2 dimensions):
    dimensions(dimensions),
    format(format)
{
}

auto CubeTextureData::loadFromFaceFiles(
    Device *device,
    const str& positiveXPath, const str& negativeXPath,
    const str& positiveYPath, const str& negativeYPath,
    const str& positiveZPath, const str& negativeZPath) -> sptr<CubeTextureData>
{
    if (STBCubeTextureData::canLoadFromFaceFiles(positiveXPath, negativeXPath, positiveYPath, negativeYPath, positiveZPath, negativeZPath))
        return STBCubeTextureData::loadFromFaceFiles(device, positiveXPath, negativeXPath, positiveYPath, negativeYPath, positiveZPath, negativeZPath);
    return panic<nullptr_t>(SL_FMT("Unsupported cube texture face files ", positiveXPath, ", ..."));
}

auto CubeTextureData::getTextureFormat() const -> TextureFormat
{
    return toTextureFormat(format);
}

CubeTextureData::CubeTextureData(TextureDataFormat format, u32 dimension):
    dimension(dimension),
    format(format)
{
}
