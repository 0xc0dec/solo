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

    class RectTextureData
    {
    public:
        static auto loadFromFile(Device *device, const std::string &path) -> sptr<RectTextureData>;

        SL_DISABLE_COPY_AND_MOVE(RectTextureData)
        virtual ~RectTextureData() {}

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
        RectTextureData() {}
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

        virtual auto getDimension(uint32_t mipLevel) const -> uint32_t = 0;

        virtual auto getData() const -> const void* = 0;
        virtual auto getData(uint32_t face) const -> const void* = 0;

        virtual auto getFormat() const -> TextureFormat = 0;

    protected:
        CubeTextureData() {}
    };
}