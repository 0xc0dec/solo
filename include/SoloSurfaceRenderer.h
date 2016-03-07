#pragma once

#include "SoloBase.h"

namespace solo
{
    class Material;
    class FrameBuffer;
    class Device;

    class SurfaceRenderer
    {
    public:
        static shared<SurfaceRenderer> create(Device* device, shared<Material> material);

        SL_NONCOPYABLE(SurfaceRenderer);
        virtual ~SurfaceRenderer() {}

        virtual void renderSurface(FrameBuffer* target) = 0;

    protected:
        SurfaceRenderer() {}
    };
}