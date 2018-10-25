/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"

namespace solo
{
    enum class TextureFormat;
    class Device;

    enum class TextureDataFormat
    {
        Red,
        RGB,
        RGBA
    };

    class Texture2DData: public NoCopyAndMove
    {
    public:
        static auto loadFromFile(Device *device, const str &path) -> sptr<Texture2DData>;
        static auto createFromMemory(u32 width, u32 height, TextureDataFormat format, const vec<u8> &data) -> sptr<Texture2DData>;

        virtual ~Texture2DData() = default;

        virtual auto getSize() const -> u32 = 0;
        virtual auto getData() const -> const void* = 0;

        auto getDimensions() const -> Vector2 { return dimensions_; }
        
        /// Returns "best suited" texture format for this data
        auto getTextureFormat() const -> TextureFormat;
        auto getFormat() const -> TextureDataFormat { return format_; }

    protected:
        Vector2 dimensions_;
        TextureDataFormat format_ = TextureDataFormat::RGBA;

        explicit Texture2DData(TextureDataFormat format, Vector2 dimensions);
    };

    class CubeTextureData: public NoCopyAndMove
    {
    public:
        static auto loadFromFaceFiles(
            Device *device,
            const str& positiveXPath, const str& negativeXPath,
            const str& positiveYPath, const str& negativeYPath,
            const str& positiveZPath, const str& negativeZPath) -> sptr<CubeTextureData>;
    
        virtual ~CubeTextureData() = default;

        virtual auto getSize() const -> u32 = 0;
        virtual auto getSize(u32 face) const -> u32 = 0; // TODO use TextureFace enum
        virtual auto getData(u32 face) const -> const void* = 0;

        auto getDimension() const -> u32 { return dimension_; }
        
        /// Returns "best suited" texture format for this data
        auto getTextureFormat() const -> TextureFormat;
        auto getFormat() const -> TextureDataFormat { return format_; }

    protected:
        u32 dimension_ = 0;
        TextureDataFormat format_ = TextureDataFormat::RGBA;

        CubeTextureData(TextureDataFormat format, u32 dimension);
    };
}