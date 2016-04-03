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

        void setData(CubeTextureFace face, ColorFormat format, const std::vector<uint8_t>& data, uint32_t width, uint32_t height);
        
        virtual void setWrapping(TextureWrapping wrapping) override final;

        auto getDepthWrapping() const -> TextureWrapping;
        void setDepthWrapping(TextureWrapping depthWrap);

    protected:
        CubeTexture() {}

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