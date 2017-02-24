/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloEffect.h"
#include "SoloDevice.h"
#include "platform/gl/SoloOpenGLEffect.h"
#include "platform/null/SoloNullEffect.h"

using namespace solo;


auto Effect::create(Device *device, const std::string &vsSrc, const std::string &fsSrc) -> sptr<Effect>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Effect>(device, vsSrc, fsSrc);
#endif
        default:
            return std::make_shared<null::Effect>();
    }
}


auto Effect::createFromPrefab(Device *device, EffectPrefab prefab) -> sptr<Effect>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return gl::Effect::create(device, prefab);
#endif
        default:
            return std::make_shared<null::Effect>();
    }
}
