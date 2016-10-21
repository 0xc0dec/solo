#pragma once

#include "SoloMaterialParameter.h"
#include "SoloOpenGLRenderer.h"

namespace solo
{
    class Texture;
    class MaterialParameterValue;

    class OpenGLMaterialParameter final: public MaterialParameter
    {
    public:
        OpenGLMaterialParameter(Effect* effect, MaterialParameterType type, const char* name);
        ~OpenGLMaterialParameter();

        void setValue(const void* value) override final;
        void apply(const RenderContext& context) override final;

    private:
        static auto getUniformType(MaterialParameterType type) -> UniformType;

        OpenGLRenderer* renderer = nullptr;
        UniformHandle handle = EmptyUniformHandle;

        uptr<MaterialParameterValue> value;
        uint32_t count = 0;
    };
}
