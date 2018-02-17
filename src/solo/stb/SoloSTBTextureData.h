/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloTextureData.h"
#include "SoloTexture.h"
#include <stb_image.h>

namespace solo
{
    class Device;

    class STBTexture2DData final: public Texture2DData
    {
    public:
        static bool canLoadFromFile(const str &path);
        static auto loadFromFile(Device *device, const str &path) -> sptr<STBTexture2DData>;

        STBTexture2DData() = default;
        ~STBTexture2DData();

        auto getSize() const -> u32 override final { return width * height * channels; }
        auto getWidth() const -> u32 override final { return width; }
        auto getHeight() const -> u32 override final { return height; }
        auto getData() const -> const void * override final { return data; }
        auto getFormat() const -> TextureFormat override final { return format; }

    private:
        u32 channels = 0;
        u32 width = 0;
        u32 height = 0;
        TextureFormat format = TextureFormat::RGB;
        stbi_uc *data = nullptr;
    };

    class STBCubeTextureData final: public CubeTextureData
    {
    public:
        static bool canLoadFromFaceFiles(
            const str& positiveXPath, const str& negativeXPath,
			const str& positiveYPath, const str& negativeYPath,
			const str& positiveZPath, const str& negativeZPath);
        static auto loadFromFaceFiles(
            Device *device,
            const str& positiveXPath, const str& negativeXPath,
			const str& positiveYPath, const str& negativeYPath,
			const str& positiveZPath, const str& negativeZPath) -> sptr<STBCubeTextureData>;

        auto getSize() const -> u32 override final;
        auto getSize(u32 face) const -> u32 override final;
        auto getDimension() const -> u32 override final;
        auto getData(u32 face) const -> const void* override final;
        auto getFormat() const -> TextureFormat override final;

    private:
        vec<sptr<STBTexture2DData>> faces;
    };
}