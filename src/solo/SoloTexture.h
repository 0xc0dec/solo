/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"
#include "math/SoloVector2.h"
#include "SoloAsyncHandle.h"
#include "SoloEnums.h"

namespace solo
{
    class Device;
    class Texture2D;
    class Texture2DData;
    class CubeTexture;
    class CubeTextureData;

    class Texture
    {
    public:
		Texture() = delete;
        Texture(const Texture &other) = delete;
		Texture(Texture &&other) = delete;
		virtual ~Texture() = default;
    	
		auto operator=(const Texture &other) -> Texture& = delete;
		auto operator=(Texture &&other) -> Texture& = delete;

		auto format() const -> TextureFormat { return format_; }

        auto horizontalWrap() const -> TextureWrap { return horizontalWrap_; }
        auto verticalWrap() const -> TextureWrap { return verticalWrap_; }
        void setHorizontalWrap(TextureWrap wrap);
        void setVerticalWrap(TextureWrap wrap);
        virtual void setWrap(TextureWrap wrap);

        auto minFilter() const -> TextureFilter { return minFilter_; }
        auto magFilter() const -> TextureFilter { return magFilter_; }
        auto mipFilter() const -> TextureMipFilter { return mipFilter_; }
        void setFilter(TextureFilter minFilter, TextureFilter magFilter, TextureMipFilter mipFilter);

        auto anisotropyLevel() const -> float { return anisotropyLevel_; }
        void setAnisotropyLevel(float level);

    protected:
        TextureFormat format_ = TextureFormat::RGBA8;

        TextureWrap horizontalWrap_ = TextureWrap::ClampToEdge;
        TextureWrap verticalWrap_ = TextureWrap::ClampToEdge;

        TextureFilter minFilter_ = TextureFilter::Linear;
        TextureFilter magFilter_ = TextureFilter::Linear;
        TextureMipFilter mipFilter_ = TextureMipFilter::Linear;

        float anisotropyLevel_ = 1.0f;

        explicit Texture(TextureFormat format);

        virtual void rebuild() {}
    };

    class Texture2D: public Texture
    {
    public:
        static auto fromFile(Device *device, const str &path, bool generateMipmaps) -> sptr<Texture2D>;
        static auto fromFileAsync(Device *device, const str &path, bool generateMipmaps) -> sptr<AsyncHandle<Texture2D>>;
        static auto fromData(Device *device, sptr<Texture2DData> data, bool generateMipmaps) -> sptr<Texture2D>;
        static auto empty(Device *device, u32 width, u32 height, TextureFormat format) -> sptr<Texture2D>;

        auto dimensions() const -> Vector2 { return dimensions_; }

    protected:
        Vector2 dimensions_;

        explicit Texture2D(TextureFormat format, Vector2 dimensions);
    };

    class CubeTexture: public Texture
    {
    public:
        static auto fromFaceFiles(Device *device,
            const str& positiveXPath, const str& negativeXPath,
            const str& positiveYPath, const str& negativeYPath,
            const str& positiveZPath, const str& negativeZPath) -> sptr<CubeTexture>;
        static auto fromFaceFilesAsync(Device *device,
            const str& positiveXPath, const str& negativeXPath,
            const str& positiveYPath, const str& negativeYPath,
            const str& positiveZPath, const str& negativeZPath) -> sptr<AsyncHandle<CubeTexture>>;
        static auto fromData(Device *device, sptr<CubeTextureData> data) -> sptr<CubeTexture>;

        auto depthWrap() const -> TextureWrap { return depthWrap_; }
        void setDepthWrap(TextureWrap wrap);
        void setWrap(TextureWrap wrap) override;

    protected:
        TextureWrap depthWrap_ = TextureWrap::Repeat;
        u32 dimension_ = 0;

        CubeTexture(TextureFormat format, u32 dimension);
    };
}