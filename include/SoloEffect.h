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

        static auto getPrefabInstance(EffectPrefab prefab) -> sptr<Effect>;

        ~Effect();
        SL_NONCOPYABLE(Effect)

        auto getHandle() const -> ProgramHandle;

        void apply();

    private:
        Effect(const std::string& vsSrc, const std::string& fsSrc);

        static std::unordered_map<EffectPrefab, sptr<Effect>> prefabs;

        Renderer* renderer;
        ProgramHandle handle;
    };

    inline auto Effect::getHandle() const -> ProgramHandle
    {
        return handle;
    }
}
