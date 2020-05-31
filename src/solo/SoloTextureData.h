/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"
#include "SoloEnums.h"

namespace solo
{
    class Device;

    class Texture2DData
    {
    public:
        static auto fromFile(Device *device, const str &path) -> sptr<Texture2DData>;
        static auto fromMemory(u32 width, u32 height, TextureDataFormat format, const vec<u8> &data) -> sptr<Texture2DData>;

    	Texture2DData() = delete;
        Texture2DData(const Texture2DData &other) = delete;
        Texture2DData(Texture2DData &&other) = delete;
        virtual ~Texture2DData() = default;
    	
        auto operator=(const Texture2DData &other) -> Texture2DData& = delete;
        auto operator=(Texture2DData &&other) -> Texture2DData& = delete;

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

    class CubeTextureData
    {
    public:
        static auto fromFaceFiles(
            Device *device,
            const str& positiveXPath, const str& negativeXPath,
            const str& positiveYPath, const str& negativeYPath,
            const str& positiveZPath, const str& negativeZPath) -> sptr<CubeTextureData>;

    	CubeTextureData() = delete;
        CubeTextureData(const CubeTextureData &other) = delete;
        CubeTextureData(CubeTextureData &&other) = delete;
        virtual ~CubeTextureData() = default;
    	
        auto operator=(const CubeTextureData &other) -> CubeTextureData& = delete;
        auto operator=(CubeTextureData &&other) -> CubeTextureData& = delete;

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