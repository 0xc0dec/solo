#pragma once

#include "SoloCommon.h"
#include "SoloRenderer.h"
#include <unordered_map>


namespace solo
{
    enum class EffectPrefab
    {
        Skybox,
        Font
    };

    class Effect final
    {
    public:
        static auto create(const std::string& vsSrc, const std::string& fsSrc) -> sptr<Effect>;
        static auto create(EffectPrefab prefab) -> sptr<Effect>;

        ~Effect();
        SL_NONCOPYABLE(Effect)

        auto getHandle() const -> ProgramHandle;

        void apply();

    private:
        Effect(const std::string& vsSrc, const std::string& fsSrc);

        Renderer* renderer;
        ProgramHandle handle;
    };

    inline auto Effect::getHandle() const -> ProgramHandle
    {
        return handle;
    }
}
