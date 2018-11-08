/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
        static auto fromFile(Device *device, const str &path) -> sptr<Texture2DData>;
        static auto fromMemory(u32 width, u32 height, TextureDataFormat format, const vec<u8> &data) -> sptr<Texture2DData>;

        virtual ~Texture2DData() = default;

        virtual auto size() const -> u32 = 0;
        virtual auto data() const -> const void* = 0;

        auto dimensions() const -> Vector2 { return dimensions_; }
        
        /// Returns "best suited" texture format for this data
        auto textureFormat() const -> TextureFormat;
        auto format() const -> TextureDataFormat { return format_; }

    protected:
        Vector2 dimensions_;
        TextureDataFormat format_ = TextureDataFormat::RGBA;

        explicit Texture2DData(TextureDataFormat format, Vector2 dimensions);
    };

    class CubeTextureData: public NoCopyAndMove
    {
    public:
        static auto fromFaceFiles(
            Device *device,
            const str& positiveXPath, const str& negativeXPath,
            const str& positiveYPath, const str& negativeYPath,
            const str& positiveZPath, const str& negativeZPath) -> sptr<CubeTextureData>;
    
        virtual ~CubeTextureData() = default;

        virtual auto size() const -> u32 = 0;
        virtual auto faceSize(u32 face) const -> u32 = 0; // TODO use TextureFace enum
        virtual auto faceData(u32 face) const -> const void* = 0;

        auto dimension() const -> u32 { return dimension_; }
        
        /// Returns "best suited" texture format for this data
        auto textureFormat() const -> TextureFormat;
        auto format() const -> TextureDataFormat { return format_; }

    protected:
        u32 dimension_ = 0;
        TextureDataFormat format_ = TextureDataFormat::RGBA;

        CubeTextureData(TextureDataFormat format, u32 dimension);
    };
}