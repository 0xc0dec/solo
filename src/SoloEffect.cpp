#include "SoloEffect.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLPrefabShaders.h"

using namespace solo;


std::unordered_map<EffectPrefab, sptr<Effect>> Effect::prefabs;


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
    switch (Device::get()->getMode())
    {
        case DeviceMode::Stub:
            return std::shared_ptr<Effect>(new Effect("", ""));
        case DeviceMode::OpenGL:
            switch (prefab)
            {
                case EffectPrefab::Skybox: return std::shared_ptr<Effect>(new Effect(OpenGLPrefabShaders::Vertex::skybox, OpenGLPrefabShaders::Fragment::skybox));
                case EffectPrefab::Font: return std::shared_ptr<Effect>(new Effect(OpenGLPrefabShaders::Vertex::simple, OpenGLPrefabShaders::Fragment::font));
                default: SL_ASSERT(false); break;
            }
            break;
        default:
            SL_ASSERT(false);
            break;
    }
    return nullptr;
}


auto Effect::getPrefabInstance(EffectPrefab prefab) -> sptr<Effect>
{
    if (prefabs.find(prefab) == prefabs.end())
        return prefabs[prefab] = create(prefab);
    return prefabs.at(prefab);
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
