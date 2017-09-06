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
            static bool canLoadFromFile(const std::string &path);
            static auto loadFromFile(Device *device, const std::string &path) -> sptr<Texture2dData>;

            Texture2dData() {}
            ~Texture2dData();

            auto getMipLevels() const -> uint32_t override final { return 1; }

            auto getSize() const -> uint32_t override final { return width * height * bpp; }
            auto getSize(uint32_t mipLevel) const -> uint32_t override final { return getSize(); }

            auto getWidth() const -> uint32_t override final { return width; }
            auto getWidth(uint32_t mipLevel) const -> uint32_t override final { return getWidth(); }
            auto getHeight() const -> uint32_t override final { return height; }
            auto getHeight(uint32_t mipLevel) const -> uint32_t override final { return getHeight(); }

            auto getData() const -> const void * override final { return data; }

            auto getFormat() const -> TextureFormat override final { return format; }

        private:
            uint32_t bpp = 0;
            uint32_t width = 0;
            uint32_t height = 0;
            TextureFormat format = TextureFormat::RGB;
            stbi_uc *data = nullptr;
        };
    }
}