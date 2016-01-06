#pragma once

#include "SoloTexture2D.h"

namespace solo
{
    class StubTexture2D : public Texture2D
    {
    public:
        virtual void apply() override {}
        virtual void generateMipmaps() override {}

    protected:
        void applyData(ColorFormat format, const std::vector<uint8_t> &data, unsigned width, unsigned height) override {}

    private:
        friend class Texture;

        StubTexture2D() {}
    };
}