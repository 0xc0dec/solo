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
        case DeviceMode::Stub:
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
