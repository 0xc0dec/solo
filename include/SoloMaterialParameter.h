#pragma once

#include "SoloBase.h"
#include "SoloEffectVariable.h"
#include "SoloMatrix.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloRenderer.h"
#include <functional>
#include <vector>


namespace solo
{
    class Texture;
    class Material;
    struct RenderContext;

//    enum class AutoBinding
//    {
//        None,
//        WorldMatrix,
//        ViewMatrix,
//        ProjectionMatrix,
//        WorldViewMatrix,
//        ViewProjectionMatrix,
//        WorldViewProjectionMatrix,
//        InverseTransposedWorldMatrix,
//        InverseTransposedWorldViewMatrix,
//        CameraWorldPosition,
//    };

    class MaterialParameter final
    {
    public:
        SL_NONCOPYABLE(MaterialParameter)

        void setFloat(float value);
        void setFloatArray(const std::vector<float>& value);
        void setVector2(const Vector2& value);
        void setVector2Array(const std::vector<Vector2>& value);
        void setVector3(const Vector3& value);
        void setVector3Array(const std::vector<Vector3>& value);
        void setVector4(const Vector4& value);
        void setVector4Array(const std::vector<Vector4>& value);
        void setMatrix(const Matrix& value);
        void setMatrixArray(const std::vector<Matrix>& value);
        void setTexture(const shared<Texture> texture);
        void setFunction(std::function<void(EffectVariable* variable, const RenderContext& context)> func);
        void bindValue(AutoBinding autoBinding);

    private:
        friend class Material;

        enum class ValueType
        {
            None = 0,
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
            Func
        };

        MaterialParameter(const std::string& name, Material* material);
        ~MaterialParameter();

        void clearOldValue(ValueType newType);

        Material* material;
        UniformHandle uniformHandle;
        std::string name = "";

        ValueType type = ValueType::None;
        float floatValue = 0;
        Vector2 vector2Value;
        Vector3 vector3Value;
        Vector4 vector4Value;
        Matrix matrixValue;
        shared<Texture> textureValue;
        std::function<void(EffectVariable* variable, const RenderContext& context)> funcValue;
        std::vector<float> floatArrayValue;
        std::vector<Vector2> vector2ArrayValue;
        std::vector<Vector3> vector3ArrayValue;
        std::vector<Vector4> vector4ArrayValue;
        std::vector<Matrix> matrixArrayValue;

        void apply(const RenderContext& context);

        static void setWorldMatrix(EffectVariable* variable, const RenderContext& context);
        static void setViewMatrix(EffectVariable* variable, const RenderContext& context);
        static void setProjectionMatrix(EffectVariable* variable, const RenderContext& context);
        static void setCameraWorldPosition(EffectVariable* variable, const RenderContext& context);
        static void setWorldViewMatrix(EffectVariable* variable, const RenderContext& context);
        static void setViewProjectionMatrix(EffectVariable* variable, const RenderContext& context);
        static void setWorldViewProjectionMatrix(EffectVariable* variable, const RenderContext& context);
        static void setInverseTransposedWorldMatrix(EffectVariable* variable, const RenderContext& context);
        static void setInverseTransposedWorldViewMatrix(EffectVariable* variable, const RenderContext& context);
    };
}