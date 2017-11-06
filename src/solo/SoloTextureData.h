/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

namespace solo
{
    enum class TextureFormat;
    enum class CubeTextureFace;
    class Device;

    class Texture2dData
    {
    public:
        static auto loadFromFile(Device *device, const str &path) -> sptr<Texture2dData>;
        static auto createFromMemory(u32 width, u32 height, TextureFormat format, const vec<u8> &data) -> sptr<Texture2dData>;

        SL_DISABLE_COPY_AND_MOVE(Texture2dData)
        virtual ~Texture2dData() {}

        virtual auto getMipLevels() const -> u32 = 0;

        virtual auto getSize() const -> u32 = 0;
        virtual auto getSize(u32 mipLevel) const -> u32 = 0;

        virtual auto getWidth() const -> u32 = 0;
        virtual auto getWidth(u32 mipLevel) const -> u32 = 0;
        virtual auto getHeight() const -> u32 = 0;
        virtual auto getHeight(u32 mipLevel) const -> u32 = 0;

        virtual auto getData() const -> const void* = 0;

        virtual auto getFormat() const -> TextureFormat = 0;

    protected:
        Texture2dData() {}
    };

    class CubeTextureData
    {
    public:
        static auto loadFromFaceFiles(Device *device,
            const str &frontPath,
            const str &backPath,
            const str &leftPath,
            const str &rightPath,
            const str &topPath,
            const str &bottomPath) -> sptr<CubeTextureData>;

        SL_DISABLE_COPY_AND_MOVE(CubeTextureData)
        virtual ~CubeTextureData() {}

        virtual auto getMipLevels() const -> u32 = 0;

        virtual auto getSize() const -> u32 = 0;
        virtual auto getSize(u32 mipLevel) const -> u32 = 0;
        virtual auto getSize(u32 face, u32 mipLevel) const -> u32 = 0;

        virtual auto getDimension() const -> u32 = 0;
        virtual auto getDimension(u32 mipLevel) const -> u32 = 0;

        virtual auto getData() const -> const void* = 0;
        virtual auto getData(u32 face) const -> const void* = 0;

        virtual auto getFormat() const -> TextureFormat = 0;

    protected:
        CubeTextureData() {}
    };
}