#pragma once

#include "SoloSurfaceRenderer.h"

namespace solo
{
    class StubSurfaceRenderer: public SurfaceRenderer
    {
    public:
        virtual void renderSurface(RenderTarget* target) override {}

    private:
        friend class SurfaceRenderer;
        
        StubSurfaceRenderer() {}
    };
}