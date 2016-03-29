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
        static sptr<Effect> create(const std::string& vsSrc, const std::string& fsSrc);
        static sptr<Effect> create(EffectPrefab prefab);

        ~Effect();
        SL_NONCOPYABLE(Effect)

        ProgramHandle getHandle() const;

        void bind();

    private:
        Effect(const std::string& vsSrc, const std::string& fsSrc);
        Effect(EffectPrefab prefab);

        Renderer* renderer;
        ProgramHandle handle;
    };

    inline ProgramHandle Effect::getHandle() const
    {
        return handle;
    }
}
