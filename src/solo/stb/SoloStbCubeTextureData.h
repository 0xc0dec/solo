/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloTextureData.h"
#include "SoloTexture.h"
#include "SoloStbRectTextureData.h"

namespace solo
{
    class Device;

    namespace stb
    {
        class CubeTextureData final: public solo::CubeTextureData
        {
        public:
            static bool canLoadFromFaceFiles(
                const std::string &frontPath,
                const std::string &backPath,
                const std::string &leftPath,
                const std::string &rightPath,
                const std::string &topPath,
                const std::string &bottomPath);
            static auto loadFromFaceFiles(Device *device,
                const std::string &frontPath,
                const std::string &backPath,
                const std::string &leftPath,
                const std::string &rightPath,
                const std::string &topPath,
                const std::string &bottomPath) -> sptr<CubeTextureData>;

            auto getMipLevels() const -> uint32_t override;

            auto getSize() const -> uint32_t override final;
            auto getSize(uint32_t mipLevel) const -> uint32_t override final;
            auto getSize(uint32_t face, uint32_t mipLevel) const -> uint32_t override final;

            auto getDimension(uint32_t mipLevel) const -> uint32_t override final;

            auto getData() const -> const void* override final;
            auto getData(uint32_t face) const -> const void* override final;

            auto getFormat() const -> TextureFormat override final;

        private:
            std::vector<sptr<stb::RectTextureData>> faces;
        };
    }
}
