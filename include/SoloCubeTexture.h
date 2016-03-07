#pragma once

#include "SoloTexture.h"


namespace solo
{
    enum class CubeTextureFace;

    class CubeTexture final: public Texture
    {
    public:
        explicit CubeTexture(Renderer* renderer);

        virtual void apply() override final;

        void setData(CubeTextureFace face, ColorFormat format, const std::vector<uint8_t>& data, int width, int height);

        void generateMipmaps() {} // TODO

        virtual void setWrapping(TextureWrapping wrapping) override final;

        TextureWrapping getDepthWrapping() const;
        void setDepthWrapping(TextureWrapping depthWrap);

    protected:
        virtual void rebuildFlags() override final;

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