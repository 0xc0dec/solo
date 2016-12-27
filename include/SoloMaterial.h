/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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
    class MaterialParameter;
    class Camera;
    class Transform;
    enum class MaterialParameterType;

    class Material
    {
    public:
        static auto create(Device *device, sptr<Effect> effect) -> sptr<Material>;

        SL_DISABLE_COPY_AND_MOVE(Material)
        virtual ~Material() {}

        void setFloatParameter(const std::string &name, float value);
        void setFloatArrayParameter(const std::string &name, const std::vector<float> &value);
        void setVector2Parameter(const std::string &name, const Vector2 &value);
        void setVector2ArrayParameter(const std::string &name, const std::vector<Vector2> &value);
        void setVector3Parameter(const std::string &name, const Vector3 &value);
        void setVector3ArrayParameter(const std::string &name, const std::vector<Vector3> &value);
        void setVector4Parameter(const std::string &name, const Vector4 &value);
        void setVector4ArrayParameter(const std::string &name, const std::vector<Vector4> &value);
        void setMatrixParameter(const std::string &name, const Matrix &value);
        void setMatrixArrayParameter(const std::string &name, const std::vector<Matrix> &value);
        void setTextureParameter(const std::string &name, sptr<Texture> value);
        void bindWorldMatrixParameter(const std::string &name);
        void bindViewMatrixParameter(const std::string &name);
        void bindProjectionMatrixParameter(const std::string &name);
        void bindWorldViewMatrixParameter(const std::string &name);
        void bindViewProjectionMatrixParameter(const std::string &name);
        void bindWorldViewProjectionMatrixParameter(const std::string &name);
        void bindInvTransposedWorldMatrixParameter(const std::string &name);
        void bindInvTransposedWorldViewMatrixParameter(const std::string &name);
        void bindCameraWorldPositionParameter(const std::string &name);

        void apply(const Camera *camera, Transform *nodeTransform);

        auto getEffect() const -> Effect*;

        auto getPolygonMode() const -> PolygonMode;
        void setPolygonMode(PolygonMode mode);

        bool isTransparent() const;
        void setTransparent(bool enabled);

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
        explicit Material(Device *device, sptr<Effect> effect);

        virtual void applyState() = 0;
        void setParameter(const std::string &name, MaterialParameterType type, const void *value);

        Device *device = nullptr;
        sptr<Effect> effect;

        std::unordered_map<std::string, sptr<MaterialParameter>> parameters;

        FaceCull faceCull = FaceCull::CW;
        PolygonMode polygonMode = PolygonMode::Triangle;
        bool depthWrite = true;
        bool depthTest = true;
        bool transparent = false;
        BlendFactor srcBlendFactor = BlendFactor::SrcAlpha;
        BlendFactor dstBlendFactor = BlendFactor::OneMinusSrcAlpha;
        DepthFunction depthFunc = DepthFunction::Less;
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
