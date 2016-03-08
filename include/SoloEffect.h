#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"


namespace solo
{
    class EffectVariable;
    class EffectVertexAttribute;

    enum class EffectPrefab
    {
        Skybox
    };

    class Effect final
    {
    public:
        Effect(Renderer* renderer, const std::string& vsSrc, const std::string& fsSrc);
        Effect(Renderer* renderer, EffectPrefab prefab);
        ~Effect();

        SL_NONCOPYABLE(Effect)

        ProgramHandle getHandle() const;

        void bind();
        void unbind();

        EffectVariable* findVariable(const std::string& name) const;

    protected:
        Renderer* renderer;
        ProgramHandle handle;

        std::unordered_map<std::string, shared<EffectVariable>> variables;
    };

    inline ProgramHandle Effect::getHandle() const
    {
        return handle;
    }
}
