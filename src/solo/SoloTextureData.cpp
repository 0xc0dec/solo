/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloTextureData.h"
#include "stb/SoloSTBTextureData.h"

namespace solo {
    class InMemoryTexture2DData final: public Texture2DData {
    public:
        explicit InMemoryTexture2DData(Vector2 dimensions, TextureDataFormat format, const vec<u8> &data):
            Texture2DData(format, dimensions),
            data_(data) {
        }

        auto size() const -> u32 final {
            return data_.size();
        }
        auto data() const -> const void* final {
            return data_.data();
        }

    private:
        vec<u8> data_;
    };
}

using namespace solo;

static auto toTextureFormat(TextureDataFormat format) -> TextureFormat {
    switch (format) {
        case TextureDataFormat::Red:
            return TextureFormat::R8;
        case TextureDataFormat::RGB:
            return TextureFormat::RGB8;
        case TextureDataFormat::RGBA:
            return TextureFormat::RGBA8;
        default:
            panic("Texture data format not convertible to texture format");
            return TextureFormat::RGBA8;
    }
}

auto Texture2DData::fromFile(Device *device, const str &path) -> sptr<Texture2DData> {
    panicIf(!STBTexture2DData::canLoadFromFile(path), "Unsupported cube texture file ", path);
    return STBTexture2DData::fromFile(device, path);
}

auto Texture2DData::fromMemory(u32 width, u32 height, TextureDataFormat format, const vec<u8> &data) -> sptr<Texture2DData> {
    return std::make_shared<InMemoryTexture2DData>(Vector2(width, height), format, data);
}

auto Texture2DData::textureFormat() const -> TextureFormat {
    return toTextureFormat(format_);
}

Texture2DData::Texture2DData(TextureDataFormat format, Vector2 dimensions):
    dimensions_(dimensions),
    format_(format) {
}

auto CubeTextureData::fromFaceFiles(
    Device *device,
    const str &positiveXPath, const str &negativeXPath,
    const str &positiveYPath, const str &negativeYPath,
    const str &positiveZPath, const str &negativeZPath) -> sptr<CubeTextureData> {
    panicIf(!STBCubeTextureData::canLoadFromFaceFiles(
        positiveXPath, negativeXPath, positiveYPath, negativeYPath, positiveZPath, negativeZPath),
        "Unsupported cube texture face files ", positiveXPath, ", ..."
    );
    return STBCubeTextureData::fromFaceFiles(device, positiveXPath, negativeXPath, positiveYPath, negativeYPath, positiveZPath, negativeZPath);
}

auto CubeTextureData::textureFormat() const -> TextureFormat {
    return toTextureFormat(format_);
}

CubeTextureData::CubeTextureData(TextureDataFormat format, u32 dimension):
    dimension_(dimension),
    format_(format) {
}
