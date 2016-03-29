#include "SoloEffect.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLBuiltInShaders.h"

using namespace solo;


Effect::Effect(const std::string& vsSrc, const std::string& fsSrc):
    renderer(Device::get()->getRenderer())
{
    handle = renderer->createProgram(vsSrc.c_str(), fsSrc.c_str());
}


Effect::Effect(EffectPrefab prefab):
    renderer(Device::get()->getRenderer())
{
    auto mode = Device::get()->getMode();
    switch (prefab)
    {
        case EffectPrefab::Skybox:
        {
            // TODO would be better to remove platform knowledge from here
            if (mode == DeviceMode::OpenGL)
                handle = renderer->createProgram(OpenGLBuiltInShaders::vsSkybox, OpenGLBuiltInShaders::fsSkybox);
            else
                handle = EmptyProgramHandle;
            break;
        }
        default:
            SL_DEBUG_THROW(InvalidInputException, "Unknown effect prefab");
    }
}


sptr<Effect> Effect::create(const std::string& vsSrc, const std::string& fsSrc)
{
    return std::unique_ptr<Effect>(new Effect(vsSrc, fsSrc));
}


sptr<Effect> Effect::create(EffectPrefab prefab)
{
    return std::unique_ptr<Effect>(new Effect(prefab));
}


Effect::~Effect()
{
    if (!handle.empty())
        renderer->destroyProgram(handle);
}


void Effect::bind()
{
    renderer->setProgram(handle);
}
