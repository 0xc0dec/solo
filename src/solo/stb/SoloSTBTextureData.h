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

        STBTexture2DData(TextureDataFormat format, Vector2 dimensions);
        ~STBTexture2DData();

        auto getSize() const -> u32 override final { return static_cast<u32>(dimensions.x()) * static_cast<u32>(dimensions.y()) * channels; }
        auto getData() const -> const void * override final { return data; }

    private:
        u32 channels = 0;
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

        STBCubeTextureData(TextureDataFormat format, u32 dimension);

        auto getSize() const -> u32 override final;
        auto getSize(u32 face) const -> u32 override final;
        auto getData(u32 face) const -> const void* override final;

    private:
        vec<sptr<STBTexture2DData>> faces;
    };
}