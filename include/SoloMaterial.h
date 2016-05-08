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
    class MaterialParameter;
    struct RenderContext;
    enum class MaterialParameterType;

    class Material final
    {
    public:
        static auto create(sptr<Effect> effect) -> sptr<Material>; 

        ~Material() {}
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
        void bindWorldMatrixParameter(const std::string& name);
        void bindViewMatrixParameter(const std::string& name);
        void bindProjectionMatrixParameter(const std::string& name);
        void bindWorldViewMatrixParameter(const std::string& name);
        void bindViewProjectionMatrixParameter(const std::string& name);
        void bindWorldViewProjectionMatrixParameter(const std::string& name);
        void bindInverseTransposedWorldMatrixParameter(const std::string& name);
        void bindInverseTransposedWorldViewMatrixParameter(const std::string& name);
        void bindCameraWorldPositionParameter(const std::string& name);

        void apply(const RenderContext& context);

        auto getEffect() const -> Effect*;

        bool isTransparent() const;
        void setTransparent(bool enabled);

        auto getSrcBlendFactor() const -> BlendFactor;
        auto getDstBlendFactor() const -> BlendFactor;
        void setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor);

        auto getPolygonFace() const -> PolygonFace;
        void setPolygonFace(PolygonFace face);

        bool getDepthWrite() const;
        void setDepthWrite(bool enabled);

        bool getDepthTest() const;
        void setDepthTest(bool enabled);

        auto getDepthFunction() const -> DepthFunction;
        void setDepthFunction(DepthFunction func);

    private:
        explicit Material(sptr<Effect> effect);

        void applyState();
        void setParameter(const std::string& name, MaterialParameterType type, const void* value);

        Renderer* renderer;
        sptr<Effect> effect;

        std::unordered_map<std::string, sptr<MaterialParameter>> parameters;

        PolygonFace polygonFace = PolygonFace::CW;
        bool depthWrite = true; 
        bool depthTest = true;
        bool transparent = false;
        BlendFactor srcBlendFactor = BlendFactor::SrcAlpha;
        BlendFactor dstBlendFactor = BlendFactor::OneMinusSrcAlpha;
        DepthFunction depthFunc = DepthFunction::Less;
    };

    inline auto Material::getEffect() const -> Effect*
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

    inline auto Material::getSrcBlendFactor() const -> BlendFactor
    {
        return srcBlendFactor;
    }

    inline auto Material::getDstBlendFactor() const -> BlendFactor
    {
        return dstBlendFactor;
    }

    inline void Material::setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor)
    {
        srcBlendFactor = srcFactor;
        dstBlendFactor = dstFactor;
    }

    inline auto Material::getPolygonFace() const -> PolygonFace
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

    inline auto Material::getDepthFunction() const -> DepthFunction
    {
        return depthFunc;
    }

    inline void Material::setDepthTest(bool enabled)
    {
        depthTest = enabled;
    }
}
