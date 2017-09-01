/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

namespace solo
{
    enum class ImageFormat
    {
        Red,
        RGB,
        RGBA,
        Alpha
    };

    struct Image
    {
        uint32_t width;
        uint32_t height;
        ImageFormat format;
        std::vector<uint8_t> data;
    };

    // TODO Replace Image with this class
    class ImageData
    {
    public:
        ImageData(ImageData &&other) = default;
        ImageData(const ImageData &other) = delete;
        virtual ~ImageData() {}

        auto operator=(const ImageData &other) -> ImageData& = delete;
        auto operator=(ImageData &&other) -> ImageData& = default;

        virtual auto getMipLevelCount() const -> uint32_t;
        virtual auto getFaceCount() const -> uint32_t;

        virtual auto getSize() const -> uint32_t;
        virtual auto getSize(uint32_t mipLevel) const -> uint32_t;
        virtual auto getSize(uint32_t face, uint32_t mipLevel) const -> uint32_t;

        virtual auto getWidth(uint32_t mipLevel) const -> uint32_t;
        virtual auto getWidth(uint32_t face, uint32_t mipLevel) const -> uint32_t;

        virtual auto getHeight(uint32_t mipLevel) const -> uint32_t;
        virtual auto getHeight(uint32_t face, uint32_t mipLevel) const -> uint32_t;

        virtual auto getData() const -> const void*;

        virtual auto getFormat() const -> ImageFormat;
    };
}