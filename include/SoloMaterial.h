#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloMatrix.h"
#include "SoloEffect.h"
#include <unordered_map>


namespace solo
{
    class Effect;
    class Texture;
    struct RenderContext;

    enum class AutoBinding
    {
        None,
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

    class Material final
    {
    public:
        explicit Material(Renderer* renderer, sptr<Effect> effect);
        ~Material();

        SL_NONCOPYABLE(Material)

        void setFloatParameter(const std::string& name, float value);
        void setFloatArrayParameter(const std::string& name, const std::vector<float>& value);
        void setVector2Parameter(const std::string& name, const Vector2& value);
        void setVector2ArrayParameter(const std::string& name, const std::vector<Vector2>& value);
        void setVector3Parameter(const std::string& name, const Vector3& value);
        void setVector3ArrayParameter(const std::string& name, const std::vector<Vector3>& value);
        void setVector4Parameter(const std::string& name, const Vector4& value);
        void setVector4ArrayParameter(const std::string& name, const std::vector<Vector4>& value);
        void setMatrixParameter(const std::string& name, const Matrix& value);
        void setMatrixArrayParameter(const std::string& name, const std::vector<Matrix>& value);
        void setTextureParameter(const std::string& name, sptr<Texture> value);
        void setParameterAutoBinding(const std::string& name, AutoBinding autoBinding);

        void bind(const RenderContext& context);

        Effect* getEffect() const;

        bool isTransparent() const;
        void setTransparent(bool enabled);
        BlendFactor getSrcBlendFactor() const;
        BlendFactor getDstBlendFactor() const;
        void setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor);

        PolygonFace getPolygonFace() const;
        void setPolygonFace(PolygonFace face);

        bool getDepthWrite() const;
        void setDepthWrite(bool enabled);

        bool getDepthTest() const;
        void setDepthTest(bool enabled);

        DepthFunction getDepthFunction() const;
        void setDepthFunction(DepthFunction func);

    private:
        enum class ParameterValueType
        {
            Unknown = 0,
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

        // TODO this is quite ugly and slow
        struct ParameterData
        {
            UniformHandle handle;
            ParameterValueType type = ParameterValueType::Unknown;
            float floatValue = 0;
            Vector2 vector2Value;
            Vector3 vector3Value;
            Vector4 vector4Value;
            Matrix matrixValue;
            sptr<Texture> textureValue;
            std::vector<float> floatArrayValue;
            std::vector<Vector2> vector2ArrayValue;
            std::vector<Vector3> vector3ArrayValue;
            std::vector<Vector4> vector4ArrayValue;
            std::vector<Matrix> matrixArrayValue;
            AutoBinding autoBinding = AutoBinding::CameraWorldPosition;
        };

        void applyState();

        ParameterData& initParameter(const std::string& name, ParameterValueType type, UniformType uniformType, uint32_t uniformComponentCount);

        template <class T>
        void setParameter(const std::string& name, ParameterValueType type, UniformType uniformType, uint32_t uniformComponentCount,
            T ParameterData::*dataField, const T& newValue);

        Renderer* renderer;
        sptr<Effect> effect;

        std::unordered_map<std::string, ParameterData> parameters;

        PolygonFace polygonFace = PolygonFace::CW;
        bool depthWrite = true; 
        bool depthTest = true;
        bool transparent = false;
        BlendFactor srcBlendFactor = BlendFactor::SrcAlpha;
        BlendFactor dstBlendFactor = BlendFactor::OneMinusSrcAlpha;
        DepthFunction depthFunc = DepthFunction::Less;
    };

    template <class T>
    inline void Material::setParameter(const std::string& name, ParameterValueType type, UniformType uniformType, uint32_t uniformComponentCount,
        T ParameterData::*dataField, const T& newValue)
    {
        initParameter(name, type, uniformType, uniformComponentCount).*dataField = newValue;
    }

    inline Effect* Material::getEffect() const
    {
        return effect.get();
    }

    inline bool Material::isTransparent() const
    {
        return transparent;
    }

    inline void Material::setTransparent(bool enabled)
    {
        transparent = enabled;
    }

    inline BlendFactor Material::getSrcBlendFactor() const
    {
        return srcBlendFactor;
    }

    inline BlendFactor Material::getDstBlendFactor() const
    {
        return dstBlendFactor;
    }

    inline void Material::setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor)
    {
        srcBlendFactor = srcFactor;
        dstBlendFactor = dstFactor;
    }

    inline PolygonFace Material::getPolygonFace() const
    {
        return polygonFace;
    }

    inline void Material::setPolygonFace(PolygonFace face)
    {
        polygonFace = face;
    }

    inline bool Material::getDepthWrite() const
    {
        return depthWrite;
    }

    inline void Material::setDepthWrite(bool enabled)
    {
        depthWrite = enabled;
    }

    inline bool Material::getDepthTest() const
    {
        return depthTest;
    }

    inline void Material::setDepthFunction(DepthFunction func)
    {
        depthFunc = func;
    }

    inline DepthFunction Material::getDepthFunction() const
    {
        return depthFunc;
    }

    inline void Material::setDepthTest(bool enabled)
    {
        depthTest = enabled;
    }
}
