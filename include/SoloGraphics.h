#pragma once

#include "SoloBase.h"

namespace solo
{
    class RenderTarget;
    class Material;
    class Device;

    class Graphics
    {
    public:
        static shared<Graphics> create(Device *device);

        SL_NONCOPYABLE(Graphics);
        virtual ~Graphics() {}

        virtual void renderSurface(Material *material, RenderTarget *target) = 0;

    protected:
        explicit Graphics(Device *device);

        Device *device;
    };
}