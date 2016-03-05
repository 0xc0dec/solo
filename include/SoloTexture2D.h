#pragma once

#include "SoloTexture.h"
#include "SoloVector2.h"

namespace solo
{
    class Texture2D final: public Texture
    {
    public:
        explicit Texture2D(Renderer* renderer);

        virtual void apply() override;

        void setData(ColorFormat format, const std::vector<uint8_t>& data, int width, int height);

        void generateMipmaps() { } // TODO

        Vector2 getSize() const;

    private:
        Vector2 size;
    };

    inline Vector2 Texture2D::getSize() const
    {
        return size;
    }
}