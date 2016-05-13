#pragma once

#include "SoloTexture.h"


namespace solo
{
    enum class CubeTextureFace;

    class CubeTexture final: public Texture
    {
    public:
        static auto create() -> sptr<CubeTexture>;

        virtual void bind() override final;
        virtual void generateMipmaps() override final;
        virtual void setWrapping(TextureWrapping wrapping) override final;

        auto getDepthWrapping() const -> TextureWrapping;
        void setDepthWrapping(TextureWrapping depthWrap);

        void setData(CubeTextureFace face, TextureFormat format, const uint8_t* data, uint32_t width, uint32_t height);

    protected:
        CubeTexture();

        virtual void rebuildFlags() override final;

        TextureWrapping depthWrapping = TextureWrapping::Repeat;
    };

    inline auto CubeTexture::getDepthWrapping() const -> TextureWrapping
    {
        return depthWrapping;
    }

    inline void CubeTexture::setDepthWrapping(TextureWrapping wrapping)
    {
        this->depthWrapping = wrapping;
    }
}