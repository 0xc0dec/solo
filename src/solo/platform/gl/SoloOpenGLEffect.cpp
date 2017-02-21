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

#include "SoloOpenGLEffect.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloOpenGLPrefabShaders.h"

using namespace solo;


auto gl::Effect::create(Device *device, EffectPrefab prefab) -> sptr<Effect>
{
    switch (prefab)
    {
        case EffectPrefab::Skybox:
            return std::make_shared<Effect>(device, PrefabShaders::Vertex::skybox, PrefabShaders::Fragment::skybox);
        case EffectPrefab::Font:
            return std::make_shared<Effect>(device, PrefabShaders::Vertex::simple, PrefabShaders::Fragment::font);
        default:
            SL_PANIC("Unknown effect prefab");
            break;
    }
}


gl::Effect::Effect(Device *device, const std::string &vsSrc, const std::string &fsSrc)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
    handle = renderer->createProgram(vsSrc.c_str(), fsSrc.c_str());
}


gl::Effect::~Effect()
{
    renderer->destroyProgram(handle);
}


void gl::Effect::apply()
{
    renderer->setProgram(handle);
}

#endif
