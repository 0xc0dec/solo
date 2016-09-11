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
#include "platform/opengl/SoloOpenGLPrefabShaders.h"

using namespace solo;


Effect::Effect(const std::string& vsSrc, const std::string& fsSrc):
    renderer(Device::get()->getRenderer())
{
    handle = renderer->createProgram(vsSrc.c_str(), fsSrc.c_str());
}


auto Effect::create(const std::string& vsSrc, const std::string& fsSrc) -> sptr<Effect>
{
    return std::shared_ptr<Effect>(new Effect(vsSrc, fsSrc));
}


// TODO not an elegant one. Wrote this instead of an "EffectRepository" to avoid too many "entities"
auto Effect::create(EffectPrefab prefab) -> sptr<Effect>
{
    switch (Device::get()->getSetup().mode)
    {
        case DeviceMode::Null:
            return std::shared_ptr<Effect>(new Effect("", ""));
        case DeviceMode::OpenGL:
            switch (prefab)
            {
                case EffectPrefab::Skybox:
                    return std::shared_ptr<Effect>(new Effect(OpenGLPrefabShaders::Vertex::skybox, OpenGLPrefabShaders::Fragment::skybox));
                case EffectPrefab::Font:
                    return std::shared_ptr<Effect>(new Effect(OpenGLPrefabShaders::Vertex::simple, OpenGLPrefabShaders::Fragment::font));
                default:
                    SL_ERR("Unknown effect prefab");
                    break;
            }
            break;
        default:
            SL_ERR("Unknown device mode");
            break;
    }
    return nullptr;
}


Effect::~Effect()
{
    if (!handle.empty())
        renderer->destroyProgram(handle);
}


void Effect::apply()
{
    renderer->setProgram(handle);
}
