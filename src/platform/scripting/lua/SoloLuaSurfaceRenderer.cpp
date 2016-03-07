#include "SoloLuaSurfaceRenderer.h"
#include "SoloSurfaceRenderer.h"

using namespace solo;


void LuaSurfaceRenderer::renderSurface(SurfaceRenderer* renderer, LuaIntf::LuaRef target)
{
    // This shit below allows for passing nulls from Lua
    FrameBuffer* rt = nullptr;
    if (target.isValid() && target.toPtr()) // some checks figured out empirically
        rt = target.toValue<FrameBuffer*>();
    renderer->renderSurface(rt);
}
