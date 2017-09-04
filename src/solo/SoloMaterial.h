/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloRenderer.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloMatrix.h"
#include "SoloEffect.h"
#include <unordered_map>

namespace solo
{
    class Device;
    class Effect;
    class Texture;
    class Camera;
    class Transform;

    enum class FaceCull
    {
        All = 0,
        CW,
        CCW
    };

    enum class PolygonMode
    {
        Triangle,
        Wireframe,
        Points
    };

    enum class DepthFunction
    {
        Never = 0,
        Less,
        Equal,
        LEqual,
        Greater,
        NotEqual,
        GEqual,
        Always
    };

    enum class BlendFactor
    {
        Zero = 0,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantAlpha,
        OneMinusConstantAlpha,
        SrcAlphaSaturate
    };

    class Material
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Material)

        static auto create(Device *device, sptr<Effect> effect) -> sptr<Material>;

        virtual ~Material() {}

        virtual void setFloatParameter(const std::string &name, float value) = 0;
        virtual void setFloatArrayParameter(const std::string &name, const std::vector<float> &value) = 0;

        virtual void setVector2Parameter(const std::string &name, const Vector2 &value) = 0;
        virtual void setVector2ArrayParameter(const std::string &name, const std::vector<Vector2> &value) = 0;

        virtual void setVector3Parameter(const std::string &name, const Vector3 &value) = 0;
        virtual void setVector3ArrayParameter(const std::string &name, const std::vector<Vector3> &value) = 0;

        virtual void setVector4Parameter(const std::string &name, const Vector4 &value) = 0;
        virtual void setVector4ArrayParameter(const std::string &name, const std::vector<Vector4> &value) = 0;

        virtual void setMatrixParameter(const std::string &name, const Matrix &value) = 0;
        virtual void setMatrixArrayParameter(const std::string &name, const std::vector<Matrix> &value) = 0;

        virtual void setTextureParameter(const std::string &name, sptr<Texture> value) = 0;

        virtual void bindWorldMatrixParameter(const std::string &name) = 0;
        virtual void bindViewMatrixParameter(const std::string &name) = 0;
        virtual void bindProjectionMatrixParameter(const std::string &name) = 0;
        virtual void bindWorldViewMatrixParameter(const std::string &name) = 0;
        virtual void bindViewProjectionMatrixParameter(const std::string &name) = 0;
        virtual void bindWorldViewProjectionMatrixParameter(const std::string &name) = 0;
        virtual void bindInvTransposedWorldMatrixParameter(const std::string &name) = 0;
        virtual void bindInvTransposedWorldViewMatrixParameter(const std::string &name) = 0;
        virtual void bindCameraWorldPositionParameter(const std::string &name) = 0;

        auto getEffect() const -> Effect* { return effect.get(); }

        auto getPolygonMode() const -> PolygonMode { return polygonMode; }
        void setPolygonMode(PolygonMode mode) { polygonMode = mode; }

        bool getBlend() const { return blend; }
        void setBlend(bool enabled) { blend = enabled; }

        auto getSrcBlendFactor() const -> BlendFactor { return srcBlendFactor; }
        auto getDstBlendFactor() const -> BlendFactor { return dstBlendFactor; }
        void setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor);

        auto getFaceCull() const -> FaceCull { return faceCull; }
        void setFaceCull(FaceCull face) { faceCull = face; }

        bool getDepthWrite() const { return depthWrite; }
        void setDepthWrite(bool enabled) { depthWrite = enabled; }

        bool getDepthTest() const { return depthTest; }
        void setDepthTest(bool enabled) { depthTest = enabled; }

        auto getDepthFunction() const -> DepthFunction { return depthFunc; }
        void setDepthFunction(DepthFunction func) { depthFunc = func; }

    protected:
        sptr<Effect> effect;

        FaceCull faceCull = FaceCull::CW;
        PolygonMode polygonMode = PolygonMode::Triangle;
        bool depthWrite = true;
        bool depthTest = true;
        bool blend = false;
        BlendFactor srcBlendFactor = BlendFactor::SrcAlpha;
        BlendFactor dstBlendFactor = BlendFactor::OneMinusSrcAlpha;
        DepthFunction depthFunc = DepthFunction::Less;

        explicit Material(sptr<Effect> effect);
    };

    inline void Material::setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor)
    {
        srcBlendFactor = srcFactor;
        dstBlendFactor = dstFactor;
    }
}
