/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloTexture.h"

namespace solo
{
    enum class ImageFormat;
    enum class CubeTextureFace;
    class Device;

    class CubeTexture: public Texture
    {
    public:
        static auto create(Device *device) -> sptr<CubeTexture>;

        virtual void setData(CubeTextureFace face, ImageFormat format, const void *data, uint32_t width, uint32_t height) = 0;

        void setWrapping(TextureWrapping wrapping) override final;

        auto getDepthWrapping() const -> TextureWrapping;
        void setDepthWrapping(TextureWrapping depthWrap);

    protected:
        TextureWrapping depthWrapping = TextureWrapping::Repeat;

        CubeTexture();

        void rebuildFlags() override final;
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