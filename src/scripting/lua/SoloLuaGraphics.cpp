#include "SoloLuaGraphics.h"
#include "SoloGraphics.h"

using namespace solo;


void LuaGraphics::blit(Graphics* graphics, Material* material, LuaIntf::LuaRef target)
{
    FrameBuffer* rt = nullptr;
    if (target.isValid() && target.toPtr()) // some checks figured out empirically
        rt = target.toValue<FrameBuffer*>();
    graphics->blit(material, rt);
}
