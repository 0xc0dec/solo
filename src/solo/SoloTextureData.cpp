/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTextureData.h"
#include "stb/SoloStbTextureData.h"

namespace solo
{
    class InMemoryTexture2dData final: public Texture2dData
    {
    public:
        explicit InMemoryTexture2dData(u32 width, u32 height, TextureFormat format, const vec<u8> &data):
            width(width),
            height(height),
            format(format),
            data(data)
        {
        }

        auto getMipLevels() const -> u32 override final { return 1; }
        
        auto getSize() const -> u32 override final { return data.size(); }
        auto getSize(u32 mipLevel) const -> u32 override final { return getSize(); }

        auto getWidth() const -> u32 override final { return width; }
        auto getWidth(u32 mipLevel) const -> u32 override { return getWidth(); }

        auto getHeight() const -> u32 override final { return height; }
        auto getHeight(u32 mipLevel) const -> u32 override final { return getHeight(); }

        auto getData() const -> const void* override final { return data.data(); }

        auto getFormat() const -> TextureFormat override final { return format; }

    private:
        u32 width;
        u32 height;
        TextureFormat format;
        vec<u8> data;
    };
}

using namespace solo;

auto Texture2dData::loadFromFile(Device *device, const str &path) -> sptr<Texture2dData>
{
    if (stb::Texture2dData::canLoadFromFile(path))
        return stb::Texture2dData::loadFromFile(device, path);
    SL_PANIC(SL_FMT("Unsupported cube texture file ", path));
    return nullptr;
}

auto Texture2dData::createFromMemory(u32 width, u32 height, TextureFormat format,
    const vec<u8> &data) -> sptr<Texture2dData>
{
    return std::make_shared<InMemoryTexture2dData>(width, height, format, data);
}

auto CubeTextureData::loadFromFaceFiles(Device *device,
    const str &frontPath,
    const str &backPath,
    const str &leftPath,
    const str &rightPath,
    const str &topPath,
    const str &bottomPath) -> sptr<CubeTextureData>
{
    if (stb::CubeTextureData::canLoadFromFaceFiles(frontPath, backPath, leftPath, rightPath, topPath, bottomPath))
        return stb::CubeTextureData::loadFromFaceFiles(device, frontPath, backPath, leftPath, rightPath, topPath, bottomPath);
    SL_PANIC(SL_FMT("Unsupported cube texture face files ", frontPath, ", ..."));
    return nullptr;
}
