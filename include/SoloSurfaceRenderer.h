#pragma once

#include "SoloBase.h"

namespace solo
{
    enum class DeviceMode;
    class Material;
    class RenderTarget;

    class SurfaceRenderer
    {
    public:
        static shared<SurfaceRenderer> create(DeviceMode mode);

        SL_NONCOPYABLE(SurfaceRenderer);
        virtual ~SurfaceRenderer() {}

        virtual void renderSurface(Material *material, RenderTarget *target) = 0;

    protected:
        SurfaceRenderer() {}
    };
}