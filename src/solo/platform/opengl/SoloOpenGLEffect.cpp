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
#include "SoloDevice.h"
#include "SoloOpenGLPrefabShaders.h"

#ifdef SL_OPENGL_RENDERER

using namespace solo;


auto OpenGLEffect::create(EffectPrefab prefab) -> sptr<OpenGLEffect>
{
    auto device = Device::get();
    switch (prefab)
    {
        case EffectPrefab::Skybox:
            return std::make_shared<OpenGLEffect>(device, OpenGLPrefabShaders::Vertex::skybox, OpenGLPrefabShaders::Fragment::skybox);
        case EffectPrefab::Font:
            return std::make_shared<OpenGLEffect>(device, OpenGLPrefabShaders::Vertex::simple, OpenGLPrefabShaders::Fragment::font);
        default:
            SL_ERR("Unknown effect prefab");
            break;
    }
}


OpenGLEffect::OpenGLEffect(Device* device, const std::string& vsSrc, const std::string& fsSrc)
{
    renderer = dynamic_cast<OpenGLRenderer*>(device->getRenderer());
    handle = renderer->createProgram(vsSrc.c_str(), fsSrc.c_str());
}


OpenGLEffect::~OpenGLEffect()
{
    renderer->destroyProgram(handle);
}


void OpenGLEffect::apply()
{
    renderer->setProgram(handle);
}

#else
#   error OpenGL renderer is not supported on this platform
#endif
