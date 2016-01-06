#pragma once

#include "SoloGraphics.h"

namespace solo
{
    class StubGraphics: public Graphics
    {
    public:
        virtual void renderSurface(Material *material, RenderTarget *target) override {}

    private:
        friend class Graphics;

        explicit StubGraphics(Device *device): Graphics(device) {}
    };
}