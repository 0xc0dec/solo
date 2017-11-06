/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloMaterial.h"
#include "SoloOpenGLRenderer.h"
#include "SoloOpenGLEffect.h"
#include "SoloOpenGL.h"
#include "SoloMap.h"

namespace solo
{
    class Device;
    class Camera;
    class Transform;

    namespace gl
    {
        class Renderer;
        class Effect;
        class Texture;

        class Material final : public solo::Material
        {
        public:
            explicit Material(sptr<solo::Effect> effect);
            ~Material() {}

            auto getEffect() const -> solo::Effect* override final { return effect.get(); }

            void setFloatParameter(const str &name, float value) override final;
            void setVector2Parameter(const str &name, const Vector2 &value) override final;
            void setVector3Parameter(const str &name, const Vector3 &value) override final;
            void setVector4Parameter(const str &name, const Vector4 &value) override final;
            void setMatrixParameter(const str &name, const Matrix &value) override final;
            void setTextureParameter(const str &name, sptr<solo::Texture> value) override final;

            void bindParameter(const str &name, BindParameterSemantics semantics) override final;

            void applyParams(const Camera *camera, const Transform *nodeTransform) const;

        protected:
            using ParameterApplier = std::function<void(const Camera *, const Transform *)>;

            sptr<gl::Effect> effect = nullptr;

            // Maybe not the fastest, but convenient and good enough for now
            umap<str, ParameterApplier> appliers;

            void setParameter(const str &paramName, std::function<ParameterApplier(GLuint, GLint)> getApplier);
        };
    }
}

#endif
