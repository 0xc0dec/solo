/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloMaterial.h"
#include "SoloOpenGLRenderer.h"
#include "SoloOpenGLEffect.h"
#include "SoloOpenGL.h"

namespace solo
{
    class Device;
    class Camera;
    class Transform;
    class OpenGLRenderer;
    class OpenGLEffect;
    class OpenGLTexture;

    class OpenGLMaterial final : public Material
    {
    public:
        explicit OpenGLMaterial(const sptr<Effect> &effect);
    	OpenGLMaterial(const OpenGLMaterial &other) = default;
        virtual ~OpenGLMaterial() = default;

        auto effect() const -> sptr<Effect> override { return effect_; }

        auto clone() const -> sptr<Material> override { return std::make_shared<OpenGLMaterial>(*this); }
    	
        void setFloatParameter(const str &name, float value) override;
        void setVector2Parameter(const str &name, const Vector2 &value) override;
        void setVector3Parameter(const str &name, const Vector3 &value) override;
        void setVector4Parameter(const str &name, const Vector4 &value) override;
        void setMatrixParameter(const str &name, const Matrix &value) override;
        void setTextureParameter(const str &name, sptr<Texture> value) override;

        void bindParameter(const str &name, ParameterBinding binding) override;

        void applyParams(const Camera *camera, const Transform *nodeTransform) const;

    protected:
        using ParameterApplier = std::function<void(const Camera *, const Transform *)>;

        sptr<OpenGLEffect> effect_;

        // Maybe not the fastest, but convenient and good enough for now
        umap<str, ParameterApplier> appliers_;

        void setParameter(const str &paramName, const std::function<ParameterApplier(GLuint, GLint)> &getApplier);
    };
}

#endif
