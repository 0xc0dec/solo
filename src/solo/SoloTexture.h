/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <vector>

namespace solo
{
    class Device;
    class RectTexture;
    class CubeTexture;

    enum class TextureWrapping
    {
        Clamp = 0,
        Repeat
    };

    enum class TextureFiltering
    {
        Nearest = 0,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear
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

    struct TextureFlags
    {
        static const uint32_t MinFilterNearest = 1 << 0;
        static const uint32_t MinFilterLinear = 1 << 1;
        static const uint32_t MinFilterNearestMipmapNearest = 1 << 2;
        static const uint32_t MinFilterLinearMipmapNearest = 1 << 3;
        static const uint32_t MinFilterNearestMipmapLinear = 1 << 4;
        static const uint32_t MinFilterLinearMipmapLinear = 1 << 5;
        static const uint32_t MagFilterNearest = 1 << 6;
        static const uint32_t MagFilterLinear = 1 << 7;
        static const uint32_t MagFilterNearestMipmapNearest = 1 << 8;
        static const uint32_t MagFilterLinearMipmapNearest = 1 << 9;
        static const uint32_t MagFilterNearestMipmapLinear = 1 << 10;
        static const uint32_t MagFilterLinearMipmapLinear = 1 << 11;
        static const uint32_t HorizontalWrapClamp = 1 << 12;
        static const uint32_t HorizontalWrapRepeat = 1 << 13;
        static const uint32_t VerticalWrapClamp = 1 << 14;
        static const uint32_t VerticalWrapRepeat = 1 << 15;
        static const uint32_t DepthWrapClamp = 1 << 16;
        static const uint32_t DepthWrapRepeat = 1 << 17;
    };

    class Texture
    {
    public:
        static auto loadRectFromFile(Device *device, const std::string &path) -> sptr<RectTexture>;
        static auto loadCubeFromFiles(Device *device, const std::vector<std::string> &paths) -> sptr<CubeTexture>;

        SL_DISABLE_COPY_AND_MOVE(Texture)

        virtual ~Texture() {}

        virtual void generateMipmaps() = 0;

        auto getHorizontalWrapping() const -> TextureWrapping;
        auto getVerticalWrapping() const -> TextureWrapping;

        virtual void setWrapping(TextureWrapping wrap);
        void setHorizontalWrapping(TextureWrapping horizontalWrap);
        void setVerticalWrapping(TextureWrapping verticalWrap);

        auto getMinFiltering() const -> TextureFiltering;
        auto getMagFiltering() const -> TextureFiltering;

        void setFiltering(TextureFiltering filtering);
        void setMinFiltering(TextureFiltering filtering);
        void setMagFiltering(TextureFiltering filtering);

        auto getAnisotropyLevel() const -> float;
        void setAnisotropyLevel(float level);

    protected:
        uint32_t flags = 0;

        TextureWrapping horizontalWrapping = TextureWrapping::Clamp;
        TextureWrapping verticalWrapping = TextureWrapping::Clamp;

        TextureFiltering minFiltering = TextureFiltering::Linear;
        TextureFiltering magFiltering = TextureFiltering::Linear;

        float anisotropy = 1.0f;

        Texture();

        virtual void rebuildFlags();
    };

    inline auto Texture::getVerticalWrapping() const -> TextureWrapping
    {
        return verticalWrapping;
    }

    inline void Texture::setVerticalWrapping(TextureWrapping wrap)
    {
        verticalWrapping = wrap;
        rebuildFlags();
    }

    inline auto Texture::getHorizontalWrapping() const -> TextureWrapping
    {
        return horizontalWrapping;
    }

    inline void Texture::setHorizontalWrapping(TextureWrapping wrap)
    {
        horizontalWrapping = wrap;
        rebuildFlags();
    }

    inline auto Texture::getMinFiltering() const -> TextureFiltering
    {
        return minFiltering;
    }

    inline void Texture::setMinFiltering(TextureFiltering filtering)
    {
        minFiltering = filtering;
        rebuildFlags();
    }

    inline auto Texture::getMagFiltering() const -> TextureFiltering
    {
        return magFiltering;
    }

    inline void Texture::setMagFiltering(TextureFiltering filtering)
    {
        magFiltering = filtering;
        rebuildFlags();
    }

    inline void Texture::setFiltering(TextureFiltering filtering)
    {
        minFiltering = filtering;
        magFiltering = filtering;
        rebuildFlags();
    }

    inline auto Texture::getAnisotropyLevel() const -> float
    {
        return anisotropy;
    }

    inline void Texture::setAnisotropyLevel(float level)
    {
        anisotropy = level;
    }
}