/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloTexture.h"
#include "SoloVector2.h"

namespace solo
{
    class Device;

    class RectTexture: public Texture
    {
    public:
        static auto loadFromFile(Device *device, const std::string &path) -> sptr<RectTexture>;
        static sptr<RectTexture> create(Device *device, uint32_t width, uint32_t height, TextureFormat format);

        virtual void setData(const void *data) = 0;

        auto getDimensions() const -> Vector2 { return dimensions; }

    protected:
        RectTexture(uint32_t width, uint32_t height, TextureFormat format);

        TextureFormat format;
        Vector2 dimensions;
    };
}