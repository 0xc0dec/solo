#pragma once

#include "SoloMaterialParameter.h"

namespace solo
{
    class NullMaterialParameter final: public MaterialParameter
    {
    public:
        explicit NullMaterialParameter(MaterialParameterType type): MaterialParameter(type)
        {
        }

        void setValue(const void* value) override final
        {
        }

        void apply(const RenderContext& context) override final
        {
        }
    };
}