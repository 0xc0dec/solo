/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloTexture.h"
#include "SoloVector2.h"

namespace solo
{
    enum class ImageFormat;
    class Device;

    class RectTexture: public Texture
    {
    public:
        static sptr<RectTexture> create(Device *device);

        virtual void setData(ImageFormat format, const void *data, uint32_t width, uint32_t height) = 0;

        auto getSize() const -> Vector2;

    protected:
        Vector2 size;
    };

    inline auto RectTexture::getSize() const -> Vector2
    {
        return size;
    }
}