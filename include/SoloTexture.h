/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloRenderer.h"

namespace solo
{
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

    class Texture
    {
    public:
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