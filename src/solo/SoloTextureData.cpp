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
        explicit InMemoryTexture2dData(uint32_t width, uint32_t height, TextureFormat format, const std::vector<uint8_t> &data):
            width(width),
            height(height),
            format(format),
            data(data)
        {
        }

        auto getMipLevels() const -> uint32_t override final { return 1; }
        
        auto getSize() const -> uint32_t override final { return data.size(); }
        auto getSize(uint32_t mipLevel) const -> uint32_t override final { return getSize(); }

        auto getWidth() const -> uint32_t override final { return width; }
        auto getWidth(uint32_t mipLevel) const -> uint32_t override { return getWidth(); }

        auto getHeight() const -> uint32_t override final { return height; }
        auto getHeight(uint32_t mipLevel) const -> uint32_t override final { return getHeight(); }

        auto getData() const -> const void* override final { return data.data(); }

        auto getFormat() const -> TextureFormat override final { return format; }

    private:
        uint32_t width;
        uint32_t height;
        TextureFormat format;
        std::vector<uint8_t> data;
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

auto Texture2dData::createFromMemory(uint32_t width, uint32_t height, TextureFormat format,
    const std::vector<uint8_t> &data) -> sptr<Texture2dData>
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
