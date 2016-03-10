#include "SoloEffect.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLBuiltInShaders.h"

using namespace solo;


Effect::Effect(Renderer* renderer, const std::string& vsSrc, const std::string& fsSrc):
    renderer(renderer)
{
    handle = renderer->createProgram(vsSrc.c_str(), fsSrc.c_str());
}


Effect::Effect(Renderer* renderer, EffectPrefab prefab):
    renderer(renderer)
{
    auto mode = renderer->getDevice()->getMode();
    switch (prefab)
    {
        case EffectPrefab::Skybox:
        {
            // TODO would be better to remove platform knowledge from here
            if (mode == DeviceMode::OpenGL)
                handle = renderer->createProgram(OpenGLBuiltInShaders::vsSkybox, OpenGLBuiltInShaders::fsSkybox);
        }
        default:
            SL_DEBUG_FMT_THROW_IF(true, InvalidInputException, "Unknown effect prefab");
    }
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


void Effect::unbind()
{
    renderer->setProgram(EmptyProgramHandle);
}
