/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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

        virtual void bind() = 0;
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
        Texture();

        virtual void rebuildFlags();
        
        uint32_t flags = 0;
        
        TextureWrapping horizontalWrapping = TextureWrapping::Clamp;
        TextureWrapping verticalWrapping = TextureWrapping::Clamp;
        
        TextureFiltering minFiltering = TextureFiltering::Linear;
        TextureFiltering magFiltering = TextureFiltering::Linear;

        float anisotropy = 1.0f;
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