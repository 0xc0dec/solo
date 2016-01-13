#include "SoloDevice.h"
#include "platform/stub/SoloStubEffect.h"
#include "platform/opengl/SoloGLSLEffect.h"
#include "platform/opengl/SoloOpenGLBuiltInShaders.h"

using namespace solo;


shared<Effect> Effect::create(DeviceMode mode, const std::string& vsSrc, const std::string& fsSrc)
{
    if (mode == DeviceMode::OpenGL)
        return SL_NEW_SHARED(GLSLEffect, vsSrc, fsSrc);
    return SL_NEW_SHARED(StubEffect);
}


shared<Effect> Effect::createPrefab(DeviceMode mode, EffectPrefab prefab)
{
    switch (prefab)
    {
    case EffectPrefab::Skybox:
        {
            // TODO would be better to remove platform knowledge from here
            if (mode == DeviceMode::OpenGL)
                return create(mode, OpenGLBuiltInShaders::vsSkybox, OpenGLBuiltInShaders::fsSkybox);
            return create(DeviceMode::Stub, "", "");
        }
    default:
        SL_THROW_FMT(EngineException, "Unknown effect prefab");
    }
}


EffectVariable* Effect::findVariable(const std::string& name) const
{
    auto where = variables.find(name);
    if (where != variables.end())
        return where->second.get();
    return nullptr;
}


EffectVertexAttribute* Effect::findVertexAttribute(const std::string& name) const
{
    // TODO eliminate copy-paste from findVariable
    auto where = vertexAttributes.find(name);
    if (where != vertexAttributes.end())
        return where->second.get();
    return nullptr;
}