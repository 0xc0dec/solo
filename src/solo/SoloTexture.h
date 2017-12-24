/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"
#include "SoloAsyncHandle.h"

namespace solo
{
    class Device;
    class Texture2d;
    class Texture2dData;
    class CubeTexture;
    class CubeTextureData;

    enum class TextureWrap
    {
        ClampToEdge = 0,
        ClampToBorder,
        Repeat,
        MirrorRepeat
    };

    enum class TextureFilter
    {
        Nearest = 0,
        Linear
    };

    enum class TextureMipFilter
    {
        None = 0,
        Nearest,
        Linear
    };

    enum class CubeTextureFace
    {
        Front = 0,
        Back,
        Left,
        Right,
        Top,
        Bottom
    };

    enum class TextureFormat
    {
        Red,
        RGB, // Not supported in Vulkan (my driver doesn't like it)
        RGBA
    };

    class Texture
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Texture)

        virtual ~Texture() {}

        auto getHorizontalWrap() const -> TextureWrap { return horizontalWrap; }
        auto getVerticalWrap() const -> TextureWrap { return verticalWrap; }
        virtual void setWrap(TextureWrap wrap);
        void setHorizontalWrap(TextureWrap wrap);
        void setVerticalWrap(TextureWrap wrap);

        auto getMinFilter() const -> TextureFilter { return minFilter; }
        auto getMagFilter() const -> TextureFilter { return magFilter; }
        auto getMipFilter() const -> TextureMipFilter { return mipFilter; }
        void setFilter(TextureFilter minFilter, TextureFilter magFilter, TextureMipFilter mipFilter);

        auto getAnisotropyLevel() const -> float { return anisotropyLevel; }
        void setAnisotropyLevel(float level);

    protected:
        TextureWrap horizontalWrap = TextureWrap::ClampToEdge;
        TextureWrap verticalWrap = TextureWrap::ClampToEdge;

        TextureFilter minFilter = TextureFilter::Linear;
        TextureFilter magFilter = TextureFilter::Linear;
        TextureMipFilter mipFilter = TextureMipFilter::Linear;

        float anisotropyLevel = 1.0f;

        Texture();

        virtual void rebuild() {}
    };

    class Texture2d: public Texture
    {
    public:
        static auto loadFromFile(Device *device, const str &path, bool generateMipmaps) -> sptr<Texture2d>;
        static auto loadFromFileAsync(Device *device, const str &path, bool generateMipmaps) -> sptr<AsyncHandle<Texture2d>>;
        static auto createFromData(Device *device, Texture2dData *data, bool generateMipmaps) -> sptr<Texture2d>;
        static auto createEmpty(Device *device, u32 width, u32 height, TextureFormat format) -> sptr<Texture2d>;

        auto getDimensions() const -> Vector2 { return dimensions; }

    protected:
        TextureFormat format;
        Vector2 dimensions;

        explicit Texture2d(Texture2dData *data);
    };

    class CubeTexture: public Texture
    {
    public:
        static auto loadFromFaceFiles(Device *device,
            const str &frontPath,
            const str &backPath,
            const str &leftPath,
            const str &rightPath,
            const str &topPath,
            const str &bottomPath) -> sptr<CubeTexture>;
        static auto loadFromFaceFilesAsync(Device *device,
            const str &frontPath,
            const str &backPath,
            const str &leftPath,
            const str &rightPath,
            const str &topPath,
            const str &bottomPath) -> sptr<AsyncHandle<CubeTexture>>;
        static auto createFromData(Device *device, CubeTextureData *data) -> sptr<CubeTexture>;

        auto getDepthWrap() const -> TextureWrap { return depthWrap; }
        void setDepthWrap(TextureWrap wrap);
        void setWrap(TextureWrap wrap) override;

    protected:
        TextureWrap depthWrap = TextureWrap::Repeat;
        u32 dimension = 0;
        TextureFormat format = TextureFormat::RGB;

        explicit CubeTexture(CubeTextureData *data);
    };
}