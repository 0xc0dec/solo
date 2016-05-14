#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"


namespace solo
{
    class Effect;
    struct RenderContext;
    enum class UniformType;

    enum class MaterialParameterType
    {
        Float,
        FloatArray,
        Vector2,
        Vector2Array,
        Vector3,
        Vector3Array,
        Vector4,
        Vector4Array,
        Matrix,
        MatrixArray,
        Texture,
        // Auto-binding values
        WorldMatrix,
        ViewMatrix,
        ProjectionMatrix,
        WorldViewMatrix,
        ViewProjectionMatrix,
        WorldViewProjectionMatrix,
        InverseTransposedWorldMatrix,
        InverseTransposedWorldViewMatrix,
        CameraWorldPosition
    };

    class MaterialParameter
    {
    public:
        static auto create(Renderer* renderer, Effect* effect, MaterialParameterType type, const char* name) -> sptr<MaterialParameter>;

        virtual ~MaterialParameter();
        SL_NONCOPYABLE(MaterialParameter)

        auto getType() const -> MaterialParameterType;

        virtual void setValue(const void* value) = 0;
        virtual void apply(const RenderContext& context) = 0;

    protected:
        explicit MaterialParameter(Renderer* renderer, Effect* effect, MaterialParameterType type, const char* name);

        static auto getUniformType(MaterialParameterType type) -> UniformType;

        MaterialParameterType type;
        UniformHandle handle = EmptyUniformHandle;
        Renderer* renderer = nullptr;
    };
}