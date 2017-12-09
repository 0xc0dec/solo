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

namespace solo
{
    class Device;
    class Effect;
    class Texture;
    class Camera;
    class Transform;

    enum class FaceCull // TODO Rename, currently its logic is inverted
    {
        All = 0, // TODO rename to None
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

    enum class BindParameterSemantics
    {
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

    class Material
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Material)

        static auto create(Device *device, sptr<Effect> effect) -> sptr<Material>;

        virtual ~Material() {}

        virtual void setFloatParameter(const str &name, float value) = 0;
        virtual void setVector2Parameter(const str &name, const Vector2 &value) = 0;
        virtual void setVector3Parameter(const str &name, const Vector3 &value) = 0;
        virtual void setVector4Parameter(const str &name, const Vector4 &value) = 0;
        virtual void setMatrixParameter(const str &name, const Matrix &value) = 0;
        virtual void setTextureParameter(const str &name, sptr<Texture> value) = 0;

        virtual void bindParameter(const str &name, BindParameterSemantics semantics) = 0;

        virtual auto getEffect() const -> Effect* = 0;

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
        FaceCull faceCull = FaceCull::CW;
        PolygonMode polygonMode = PolygonMode::Triangle;
        bool depthWrite = true;
        bool depthTest = true;
        bool blend = false;
        BlendFactor srcBlendFactor = BlendFactor::SrcAlpha;
        BlendFactor dstBlendFactor = BlendFactor::OneMinusSrcAlpha;
        DepthFunction depthFunc = DepthFunction::Less;

        Material() {}
    };

    inline void Material::setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor)
    {
        srcBlendFactor = srcFactor;
        dstBlendFactor = dstFactor;
    }
}
