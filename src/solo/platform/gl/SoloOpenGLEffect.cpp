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
            return std::make_shared<Effect>(device,
                PrefabShaders::Vertex::skybox, std::strlen(PrefabShaders::Vertex::skybox),
                PrefabShaders::Fragment::skybox, std::strlen(PrefabShaders::Fragment::skybox));
        case EffectPrefab::Font:
            return std::make_shared<Effect>(device,
                PrefabShaders::Vertex::positionAndTexCoord, std::strlen(PrefabShaders::Vertex::positionAndTexCoord),
                PrefabShaders::Fragment::font, std::strlen(PrefabShaders::Fragment::font));
        default:
            SL_PANIC("Unknown effect prefab");
            break;
    }
}


gl::Effect::Effect(Device *device, const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
    handle = renderer->createProgram(vsSrc, vsSrcLen, fsSrc, fsSrcLen);
}


gl::Effect::~Effect()
{
    renderer->destroyProgram(handle);
}


#endif
