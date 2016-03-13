#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"


namespace solo
{
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

    protected:
        Renderer* renderer;
        ProgramHandle handle;
    };

    inline ProgramHandle Effect::getHandle() const
    {
        return handle;
    }
}
