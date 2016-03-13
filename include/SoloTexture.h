#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"


namespace solo
{
    class Texture2D;
    class CubeTexture;
    enum class DeviceMode;

    enum class TextureWrapping
    {
        Clamp,
        Repeat
    };

    enum class TextureFiltering
    {
        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear
    };

    class Texture
    {
    public:
        Texture(Renderer* renderer);
        virtual ~Texture();

        SL_NONCOPYABLE(Texture)

        virtual void apply() = 0;
        virtual void generateMipmaps() = 0;

        TextureWrapping getHorizontalWrapping() const;
        TextureWrapping getVerticalWrapping() const;

        virtual void setWrapping(TextureWrapping wrap);
        void setHorizontalWrapping(TextureWrapping horizontalWrap);
        void setVerticalWrapping(TextureWrapping verticalWrap);

        TextureFiltering getMinFiltering() const;
        TextureFiltering getMagFiltering() const;

        void setFiltering(TextureFiltering filtering);
        void setMinFiltering(TextureFiltering filtering);
        void setMagFiltering(TextureFiltering filtering);

        float getAnisotropyLevel() const;
        void setAnisotropyLevel(float level);

        TextureHandle getHandle() const;

    protected:
        virtual void rebuildFlags();

        Renderer* renderer;
        
        int flags = 0;
        TextureHandle handle = EmptyTextureHandle;

        TextureWrapping horizontalWrapping = TextureWrapping::Repeat;
        TextureWrapping verticalWrapping = TextureWrapping::Repeat;
        
        TextureFiltering minFiltering = TextureFiltering::Linear;
        TextureFiltering magFiltering = TextureFiltering::Linear;

        float anisotropy = 1.0f;
    };

    inline TextureWrapping Texture::getVerticalWrapping() const
    {
        return verticalWrapping;
    }

    inline void Texture::setVerticalWrapping(TextureWrapping wrap)
    {
        verticalWrapping = wrap;
        rebuildFlags();
    }

    inline TextureWrapping Texture::getHorizontalWrapping() const
    {
        return horizontalWrapping;
    }

    inline void Texture::setHorizontalWrapping(TextureWrapping wrap)
    {
        horizontalWrapping = wrap;
        rebuildFlags();
    }

    inline TextureFiltering Texture::getMinFiltering() const
    {
        return minFiltering;
    }

    inline void Texture::setMinFiltering(TextureFiltering filtering)
    {
        minFiltering = filtering;
        rebuildFlags();
    }

    inline TextureFiltering Texture::getMagFiltering() const
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

    inline float Texture::getAnisotropyLevel() const
    {
        return anisotropy;
    }

    inline void Texture::setAnisotropyLevel(float level)
    {
        anisotropy = level;
    }

    inline TextureHandle Texture::getHandle() const
    {
        return handle;
    }
}