#pragma once

#include "SoloEffectRepository.h"


namespace solo
{
    class OpenGLEffectRepository final: public EffectRepository
    {
    public:
        virtual auto getDefaultSkyboxEffect() -> sptr<Effect> override final;
        virtual auto getDefaultFontEffect() -> sptr<Effect> override final;

    private:
        friend class EffectRepository;

        OpenGLEffectRepository() {}

        sptr<Effect> fontEffect;
        sptr<Effect> skyboxEffect;
    };
}