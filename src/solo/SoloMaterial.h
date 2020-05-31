/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"
#include "SoloRenderer.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloMatrix.h"
#include "SoloEffect.h"
#include "SoloEnums.h"

namespace solo
{
    class Device;
    class Effect;
    class Texture;
    class Camera;
    class Transform;

    class Material
    {
    public:
        static auto fromEffect(Device *device, sptr<Effect> effect) -> sptr<Material>;

        Material(Material &&other) = delete;
        virtual ~Material() = default;
    	
        auto operator=(const Material &other) -> Material& = delete;
        auto operator=(Material &&other) -> Material& = delete;

        virtual void setFloatParameter(const str &name, float value) = 0;
        virtual void setVector2Parameter(const str &name, const Vector2 &value) = 0;
        virtual void setVector3Parameter(const str &name, const Vector3 &value) = 0;
        virtual void setVector4Parameter(const str &name, const Vector4 &value) = 0;
        virtual void setMatrixParameter(const str &name, const Matrix &value) = 0;
        virtual void setTextureParameter(const str &name, sptr<Texture> value) = 0;

        virtual void bindParameter(const str &name, ParameterBinding binding) = 0;

        virtual auto effect() const -> sptr<Effect> = 0;

    	virtual auto clone() const -> sptr<Material> = 0;

        auto polygonMode() const -> PolygonMode { return polygonMode_; }
        void setPolygonMode(PolygonMode mode) { polygonMode_ = mode; }

        bool hasBlend() const { return blend_; }
        void setBlend(bool enabled) { blend_ = enabled; }

        auto srcBlendFactor() const -> BlendFactor { return srcBlendFactor_; }
        auto dstBlendFactor() const -> BlendFactor { return dstBlendFactor_; }
        void setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor);

        auto faceCull() const -> FaceCull { return faceCull_; }
        void setFaceCull(FaceCull face) { faceCull_ = face; }

        bool hasDepthWrite() const { return depthWrite_; }
        void setDepthWrite(bool enabled) { depthWrite_ = enabled; }

        bool hasDepthTest() const { return depthTest_; }
        void setDepthTest(bool enabled) { depthTest_ = enabled; }

        auto depthFunction() const -> DepthFunction { return depthFunc_; }
        void setDepthFunction(DepthFunction func) { depthFunc_ = func; }

    protected:
        FaceCull faceCull_ = FaceCull::Back;
        PolygonMode polygonMode_ = PolygonMode::Fill;
        bool depthWrite_ = true;
        bool depthTest_ = true;
        bool blend_ = false;
        BlendFactor srcBlendFactor_ = BlendFactor::SrcAlpha;
        BlendFactor dstBlendFactor_ = BlendFactor::OneMinusSrcAlpha;
        DepthFunction depthFunc_ = DepthFunction::Less;

        Material() = default;
    	Material(const Material &other) = default;
    };
}
