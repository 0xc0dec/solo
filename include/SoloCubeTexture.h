#pragma once

#include "SoloTexture.h"
#include "SoloImage.h"

namespace solo
{
    enum class CubeTextureFace
    {
        Front = 0,
        Back = 1,
        Left = 2,
        Right = 3,
        Top = 4,
        Bottom = 5
    };

    class CubeTexture: public Texture
    {
    public:
        virtual void setData(CubeTextureFace face, ColorFormat format, const std::vector<uint8_t>& data, int width, int height) = 0;

        virtual void generateMipmaps() = 0;

        virtual void setWrapping(TextureWrapping wrapping) override;

        TextureWrapping getDepthWrapping() const;
        void setDepthWrapping(TextureWrapping depthWrap);

    protected:
        CubeTexture() {}

        TextureWrapping depthWrapping = TextureWrapping::Repeat;
    };

    inline TextureWrapping CubeTexture::getDepthWrapping() const
    {
        return depthWrapping;
    }

    inline void CubeTexture::setDepthWrapping(TextureWrapping wrapping)
    {
        this->depthWrapping = wrapping;
    }
}