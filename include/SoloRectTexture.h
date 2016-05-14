#pragma once

#include "SoloTexture.h"
#include "SoloVector2.h"


namespace solo
{
    class RectTexture final: public Texture
    {
    public:
        static sptr<RectTexture> create();

        virtual void bind() override final;
        virtual void generateMipmaps() override final;

        void setData(TextureFormat format, const uint8_t* data, uint32_t width, uint32_t height);

        auto getSize() const -> Vector2;

    private:
        Vector2 size;
    };

    inline auto RectTexture::getSize() const -> Vector2
    {
        return size;
    }
}