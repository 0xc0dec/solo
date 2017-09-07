/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <vector>

namespace solo
{
    enum class TextureFormat;
    enum class CubeTextureFace;
    class Device;

    class Texture2dData
    {
    public:
        static auto loadFromFile(Device *device, const std::string &path) -> sptr<Texture2dData>;
        static auto createFromMemory(uint32_t width, uint32_t height, TextureFormat format, const std::vector<uint8_t> &data) -> sptr<Texture2dData>;

        SL_DISABLE_COPY_AND_MOVE(Texture2dData)
        virtual ~Texture2dData() {}

        virtual auto getMipLevels() const -> uint32_t = 0;

        virtual auto getSize() const -> uint32_t = 0;
        virtual auto getSize(uint32_t mipLevel) const -> uint32_t = 0;

        virtual auto getWidth() const -> uint32_t = 0;
        virtual auto getWidth(uint32_t mipLevel) const -> uint32_t = 0;
        virtual auto getHeight() const -> uint32_t = 0;
        virtual auto getHeight(uint32_t mipLevel) const -> uint32_t = 0;

        virtual auto getData() const -> const void* = 0;

        virtual auto getFormat() const -> TextureFormat = 0;

    protected:
        Texture2dData() {}
    };

    class CubeTextureData
    {
    public:
        static auto loadFromFaceFiles(Device *device,
            const std::string &frontPath,
            const std::string &backPath,
            const std::string &leftPath,
            const std::string &rightPath,
            const std::string &topPath,
            const std::string &bottomPath) -> sptr<CubeTextureData>;

        SL_DISABLE_COPY_AND_MOVE(CubeTextureData)
        virtual ~CubeTextureData() {}

        virtual auto getMipLevels() const -> uint32_t = 0;

        virtual auto getSize() const -> uint32_t = 0;
        virtual auto getSize(uint32_t mipLevel) const -> uint32_t = 0;
        virtual auto getSize(uint32_t face, uint32_t mipLevel) const -> uint32_t = 0;

        virtual auto getDimension() const -> uint32_t = 0;
        virtual auto getDimension(uint32_t mipLevel) const -> uint32_t = 0;

        virtual auto getData() const -> const void* = 0;
        virtual auto getData(uint32_t face) const -> const void* = 0;

        virtual auto getFormat() const -> TextureFormat = 0;

    protected:
        CubeTextureData() {}
    };
}