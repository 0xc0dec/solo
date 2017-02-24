/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
