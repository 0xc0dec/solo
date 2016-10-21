#pragma once

#include "SoloMaterial.h"

namespace solo
{
    class NullMaterial final: public Material
    {
    public:
        explicit NullMaterial(const sptr<Effect>& effect): Material(effect)
        {
        }

    protected:
        void applyState() override final
        {
        }
    };
}