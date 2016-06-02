#pragma once

#include "SoloEffectRepository.h"


namespace solo
{
    class OpenGLEffectRepository final: public EffectRepository
    {
    public:
        OpenGLEffectRepository() {}

        auto getDefaultSkyboxEffect() -> sptr<Effect> override final;
        auto getDefaultFontEffect() -> sptr<Effect> override final;

    private:
        sptr<Effect> fontEffect;
        sptr<Effect> skyboxEffect;
    };
}