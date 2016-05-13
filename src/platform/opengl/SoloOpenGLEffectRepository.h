#pragma once

#include "SoloEffectRepository.h"


namespace solo
{
    class OpenGLEffectRepository final: public EffectRepository
    {
    public:
        OpenGLEffectRepository() {}

        virtual auto getDefaultSkyboxEffect() -> sptr<Effect> override final;
        virtual auto getDefaultFontEffect() -> sptr<Effect> override final;

    private:
        sptr<Effect> fontEffect;
        sptr<Effect> skyboxEffect;
    };
}