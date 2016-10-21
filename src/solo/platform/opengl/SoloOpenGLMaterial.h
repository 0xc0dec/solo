#pragma once

#include "SoloMaterial.h"

namespace solo
{
    class OpenGLRenderer;

    class OpenGLMaterial final: public Material
    {
    public:
        explicit OpenGLMaterial(sptr<Effect> effect);

    protected:
        void applyState() override final;

        OpenGLRenderer* renderer = nullptr;
    };
}
