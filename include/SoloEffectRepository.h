#pragma once

#include "SoloBase.h"


namespace solo
{
    class DeviceToken;
    class Effect;
    enum class DeviceMode;

    class EffectRepository
    {
    public:
        static auto create(const DeviceToken&) -> uptr<EffectRepository>;

        virtual ~EffectRepository() {}
        SL_NONCOPYABLE(EffectRepository)

        virtual auto getDefaultSkyboxEffect() -> sptr<Effect> = 0;
        virtual auto getDefaultFontEffect() -> sptr<Effect> = 0;

    protected:
        EffectRepository() {}
    };
}