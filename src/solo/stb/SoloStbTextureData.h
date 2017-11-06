/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloTextureData.h"
#include "SoloTexture.h"
#include <stb_image.h>

namespace solo
{
    class Device;

    namespace stb
    {
        class Texture2dData final: public solo::Texture2dData
        {
        public:
            static bool canLoadFromFile(const str &path);
            static auto loadFromFile(Device *device, const str &path) -> sptr<Texture2dData>;

            Texture2dData() {}
            ~Texture2dData();

            auto getMipLevels() const -> u32 override final { return 1; }

            auto getSize() const -> u32 override final { return width * height * bpp; }
            auto getSize(u32 mipLevel) const -> u32 override final { return getSize(); }

            auto getWidth() const -> u32 override final { return width; }
            auto getWidth(u32 mipLevel) const -> u32 override final { return getWidth(); }
            auto getHeight() const -> u32 override final { return height; }
            auto getHeight(u32 mipLevel) const -> u32 override final { return getHeight(); }

            auto getData() const -> const void * override final { return data; }

            auto getFormat() const -> TextureFormat override final { return format; }

        private:
            u32 bpp = 0;
            u32 width = 0;
            u32 height = 0;
            TextureFormat format = TextureFormat::RGB;
            stbi_uc *data = nullptr;
        };

        class CubeTextureData final: public solo::CubeTextureData
        {
        public:
            static bool canLoadFromFaceFiles(
                const str &frontPath,
                const str &backPath,
                const str &leftPath,
                const str &rightPath,
                const str &topPath,
                const str &bottomPath);
            static auto loadFromFaceFiles(Device *device,
                const str &frontPath,
                const str &backPath,
                const str &leftPath,
                const str &rightPath,
                const str &topPath,
                const str &bottomPath) -> sptr<CubeTextureData>;

            auto getMipLevels() const -> u32 override;

            auto getSize() const -> u32 override final;
            auto getSize(u32 mipLevel) const -> u32 override final;
            auto getSize(u32 face, u32 mipLevel) const -> u32 override final;

            auto getDimension() const -> u32 override final;
            auto getDimension(u32 mipLevel) const -> u32 override final;

            auto getData() const -> const void* override final;
            auto getData(u32 face) const -> const void* override final;

            auto getFormat() const -> TextureFormat override final;

        private:
            vec<sptr<stb::Texture2dData>> faces;
        };
    }
}