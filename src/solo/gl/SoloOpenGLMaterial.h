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

        void bindFloatParameter(const str &name, const std::function<float()> &valueGetter) override;
        void bindVector2Parameter(const str &name, const std::function<Vector2()> &valueGetter) override;
    	void bindVector3Parameter(const str &name, const std::function<Vector3()> &valueGetter) override;
    	void bindVector4Parameter(const str &name, const std::function<Vector4()> &valueGetter) override;
    	void bindMatrixParameter(const str &name, const std::function<Matrix()> &valueGetter) override;
    	
        void bindParameter(const str &name, ParameterBinding binding) override;

        void applyParams(const Camera *camera, const Transform *nodeTransform) const;

    protected:
        using ParameterWriter = std::function<void(GLuint, GLuint, const Camera*, const Transform*)>;

    	struct ParameterDescriptor
    	{
    		GLuint location;
    		GLuint samplerIndex;
    		ParameterWriter write;
    	};

        sptr<OpenGLEffect> effect_;

        // TODO More optimize
        umap<str, ParameterWriter> writers_;
    	umap<str, ParameterDescriptor> descriptors_;

    	void saveParameter(const str &paramName, const ParameterWriter &writer);
    };
}

#endif
