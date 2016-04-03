#pragma once

#include "SoloTexture.h"
#include "SoloVector2.h"


namespace solo
{
    class Texture2D final: public Texture
    {
    public:
        static sptr<Texture2D> create();

        virtual void bind() override final;
        virtual void generateMipmaps() override final;

        void setData(ColorFormat format, const std::vector<uint8_t>& data, uint32_t width, uint32_t height);

        auto getSize() const -> Vector2;

    private:
        Vector2 size;
    };

    inline auto Texture2D::getSize() const -> Vector2
    {
        return size;
    }
}