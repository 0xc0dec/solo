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
        static auto create(const std::string& vsSrc, const std::string& fsSrc) -> sptr<Effect>;
        static auto create(EffectPrefab prefab) -> sptr<Effect>;

        ~Effect();
        SL_NONCOPYABLE(Effect)

        auto getHandle() const ->ProgramHandle;

        void apply();

    private:
        Effect(const std::string& vsSrc, const std::string& fsSrc);
        Effect(EffectPrefab prefab);

        Renderer* renderer;
        ProgramHandle handle;
    };

    inline auto Effect::getHandle() const -> ProgramHandle
    {
        return handle;
    }
}
