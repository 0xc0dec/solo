/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloTexture.h"

namespace solo
{
    enum class TextureFormat;
    class CubeTextureData;
    class Device;

    class CubeTexture: public Texture
    {
    public:
        static auto loadFromFaceFiles(Device *device,
            const std::string &frontPath,
            const std::string &backPath,
            const std::string &leftPath,
            const std::string &rightPath,
            const std::string &topPath,
            const std::string &bottomPath) -> sptr<CubeTexture>;
        static auto create(Device *device, uint32_t width, uint32_t height, uint32_t depth, TextureFormat format) -> sptr<CubeTexture>;

        void setWrapping(TextureWrapping wrapping) override final;

        auto getDepthWrapping() const -> TextureWrapping { return depthWrapping; }
        void setDepthWrapping(TextureWrapping depthWrap);

    protected:
        TextureWrapping depthWrapping = TextureWrapping::Repeat;
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 0;
        TextureFormat format = TextureFormat::RGB;

        CubeTexture(uint32_t width, uint32_t height, uint32_t depth, TextureFormat format);

        virtual void setData(CubeTextureData *data) = 0;

        void rebuildFlags() override final;
    };

    inline void CubeTexture::setDepthWrapping(TextureWrapping wrapping)
    {
        this->depthWrapping = wrapping;
        rebuildFlags();
    }
}