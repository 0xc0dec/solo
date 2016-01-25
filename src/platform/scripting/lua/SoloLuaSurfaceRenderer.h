#pragma once

#include <LuaIntf.h>

namespace solo
{
    class SurfaceRenderer;

    class LuaSurfaceRenderer
    {
    public:
        static void renderSurface(SurfaceRenderer* renderer, LuaIntf::LuaRef target);
    };
}