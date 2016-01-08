#pragma once

#include "SoloBase.h"

namespace solo
{
    enum class DeviceMode;
    class Material;
    class RenderTarget;
    class Device;

    class SurfaceRenderer
    {
    public:
        static shared<SurfaceRenderer> create(Device *device, shared<Material> material);

        SL_NONCOPYABLE(SurfaceRenderer);
        virtual ~SurfaceRenderer() {}

        virtual void renderSurface(RenderTarget *target) = 0;

    protected:
        SurfaceRenderer() {}
    };
}