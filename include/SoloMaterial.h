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

        auto getEffect() const -> Effect*;

        auto getPolygonMode() const -> PolygonMode;
        void setPolygonMode(PolygonMode mode);

        bool getBlend() const;
        void setBlend(bool enabled);

        auto getSrcBlendFactor() const -> BlendFactor;
        auto getDstBlendFactor() const -> BlendFactor;
        void setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor);

        auto getFaceCull() const -> FaceCull;
        void setFaceCull(FaceCull face);

        bool getDepthWrite() const;
        void setDepthWrite(bool enabled);

        bool getDepthTest() const;
        void setDepthTest(bool enabled);

        auto getDepthFunction() const -> DepthFunction;
        void setDepthFunction(DepthFunction func);

    protected:
        Device *device = nullptr;
        sptr<Effect> effect;

        FaceCull faceCull = FaceCull::CW;
        PolygonMode polygonMode = PolygonMode::Triangle;
        bool depthWrite = true;
        bool depthTest = true;
        bool blend = false;
        BlendFactor srcBlendFactor = BlendFactor::SrcAlpha;
        BlendFactor dstBlendFactor = BlendFactor::OneMinusSrcAlpha;
        DepthFunction depthFunc = DepthFunction::Less;

        explicit Material(Device *device, sptr<Effect> effect);

        virtual void applyImpl(const Camera *camera, Transform *nodeTransform) = 0;
    };

    inline auto Material::getEffect() const -> Effect *
    {
        return effect.get();
    }

    inline auto Material::getPolygonMode() const -> PolygonMode
    {
        return polygonMode;
    }

    inline void Material::setPolygonMode(PolygonMode mode)
    {
        polygonMode = mode;
    }

    inline bool Material::getBlend() const
    {
        return blend;
    }

    inline void Material::setBlend(bool enabled)
    {
        blend = enabled;
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

    inline auto Material::getFaceCull() const -> FaceCull
    {
        return faceCull;
    }

    inline void Material::setFaceCull(FaceCull face)
    {
        faceCull = face;
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
