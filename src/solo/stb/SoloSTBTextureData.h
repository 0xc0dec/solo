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

    class STBTexture2dData final: public Texture2dData
    {
    public:
        static bool canLoadFromFile(const str &path);
        static auto loadFromFile(Device *device, const str &path) -> sptr<STBTexture2dData>;

        STBTexture2dData() {}
        ~STBTexture2dData();

        auto getSize() const -> u32 override final { return width * height * bpp; }
        auto getWidth() const -> u32 override final { return width; }
        auto getHeight() const -> u32 override final { return height; }
        auto getData() const -> const void * override final { return data; }
        auto getFormat() const -> TextureFormat override final { return format; }

    private:
        u32 bpp = 0;
        u32 width = 0;
        u32 height = 0;
        TextureFormat format = TextureFormat::RGB;
        stbi_uc *data = nullptr;
    };

    class STBCubeTextureData final: public CubeTextureData
    {
    public:
        static bool canLoadFromFaceFiles(
            const str &frontPath,
            const str &backPath,
            const str &leftPath,
            const str &rightPath,
            const str &topPath,
            const str &bottomPath);
        static auto loadFromFaceFiles(
            Device *device,
            const str &frontPath,
            const str &backPath,
            const str &leftPath,
            const str &rightPath,
            const str &topPath,
            const str &bottomPath) -> sptr<STBCubeTextureData>;

        auto getSize() const -> u32 override final;
        auto getSize(u32 face) const -> u32 override final;
        auto getDimension() const -> u32 override final;
        auto getData(u32 face) const -> const void* override final;
        auto getFormat() const -> TextureFormat override final;

    private:
        vec<sptr<STBTexture2dData>> faces;
    };
}