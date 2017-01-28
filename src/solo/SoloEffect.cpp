/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloEffect.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLEffect.h"
#include "platform/null/SoloNullEffect.h"

using namespace solo;


auto Effect::create(Device *device, const std::string &vsSrc, const std::string &fsSrc) -> sptr<Effect>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLEffect>(device, vsSrc, fsSrc);
#endif
        default:
            return std::make_shared<NullEffect>();
    }
}


auto Effect::create(Device *device, EffectPrefab prefab) -> sptr<Effect>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return OpenGLEffect::create(device, prefab);
#endif
        default:
            return std::make_shared<NullEffect>();
    }
}
