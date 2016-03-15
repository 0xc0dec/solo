#pragma once

#include <LuaIntf.h>

namespace solo
{
    class Material;
    class Graphics;

    class LuaGraphics
    {
    public:
        static void blit(Graphics* graphics, Material* material, LuaIntf::LuaRef target);
    };
}