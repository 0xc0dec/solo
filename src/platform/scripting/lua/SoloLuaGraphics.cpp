#include "SoloLuaGraphics.h"
#include "SoloGraphics.h"

using namespace solo;


void LuaGraphics::renderSurface(Graphics *graphics, Material *material, LuaIntf::LuaRef target)
{
    RenderTarget *rt = nullptr;
    if (target.isValid() && target.toPtr()) // some checks figured out empirically
        rt = target.toValue<RenderTarget *>();
    graphics->renderSurface(material, rt);
}
