#pragma once

#include "SoloEffect.h"

namespace solo
{
    class StubEffectVariable;

    class StubEffect: public Effect
    {
    public:
        virtual void bind() override {}
        virtual void unbind() override {}

        virtual EffectVariable *findVariable(const std::string &name) const override;

    private:
        friend class Effect;

        StubEffect();

        shared<EffectVariable> variable;
    };
}