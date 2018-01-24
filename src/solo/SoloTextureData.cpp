/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTextureData.h"
#include "stb/SoloSTBTextureData.h"

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

        auto getSize() const -> u32 override final { return data.size(); }
        auto getWidth() const -> u32 override final { return width; }
        auto getHeight() const -> u32 override final { return height; }
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
    if (STBTexture2dData::canLoadFromFile(path))
        return STBTexture2dData::loadFromFile(device, path);
    return panic<nullptr_t>(SL_FMT("Unsupported cube texture file ", path));
}

auto Texture2dData::createFromMemory(u32 width, u32 height, TextureFormat format,
    const vec<u8> &data) -> sptr<Texture2dData>
{
    return std::make_shared<InMemoryTexture2dData>(width, height, format, data);
}

auto CubeTextureData::loadFromFaceFiles(
    Device *device,
    const str &frontPath,
    const str &backPath,
    const str &leftPath,
    const str &rightPath,
    const str &topPath,
    const str &bottomPath) -> sptr<CubeTextureData>
{
    if (STBCubeTextureData::canLoadFromFaceFiles(frontPath, backPath, leftPath, rightPath, topPath, bottomPath))
        return STBCubeTextureData::loadFromFaceFiles(device, frontPath, backPath, leftPath, rightPath, topPath, bottomPath);
    return panic<nullptr_t>(SL_FMT("Unsupported cube texture face files ", frontPath, ", ..."));
}
