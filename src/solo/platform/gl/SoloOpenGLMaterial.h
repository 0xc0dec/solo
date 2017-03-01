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
#include <unordered_map>

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

            void setFloatParameter(const std::string &name, float value) override final;
            void setFloatArrayParameter(const std::string &name, const std::vector<float> &value) override final;

            void setVector2Parameter(const std::string &name, const Vector2 &value) override final;
            void setVector2ArrayParameter(const std::string &name, const std::vector<Vector2> &value) override final;

            void setVector3Parameter(const std::string &name, const Vector3 &value) override final;
            void setVector3ArrayParameter(const std::string &name, const std::vector<Vector3> &value) override final;

            void setVector4Parameter(const std::string &name, const Vector4 &value) override final;
            void setVector4ArrayParameter(const std::string &name, const std::vector<Vector4> &value) override final;

            void setMatrixParameter(const std::string &name, const Matrix &value) override final;
            void setMatrixArrayParameter(const std::string &name, const std::vector<Matrix> &value) override final;

            void setTextureParameter(const std::string &name, sptr<solo::Texture> value) override final;

            void bindWorldMatrixParameter(const std::string &name) override final;
            void bindViewMatrixParameter(const std::string &name) override final;
            void bindProjectionMatrixParameter(const std::string &name) override final;
            void bindWorldViewMatrixParameter(const std::string &name) override final;
            void bindViewProjectionMatrixParameter(const std::string &name) override final;
            void bindWorldViewProjectionMatrixParameter(const std::string &name) override final;
            void bindInvTransposedWorldMatrixParameter(const std::string &name) override final;
            void bindInvTransposedWorldViewMatrixParameter(const std::string &name) override final;
            void bindCameraWorldPositionParameter(const std::string &name) override final;

            void applyParams(const Camera *camera, const Transform *nodeTransform) const;

        protected:
            sptr<Effect> effect = nullptr;

            std::unordered_map<std::string, GLint> uniformLocations;
            std::unordered_map<std::string, uint32_t> applierIndices;
            std::unordered_map<std::string, GLint> uniformIndexes;

            // Note: maybe not the fastest, but convenient and good enough for now
            std::vector<std::function<void(const Camera *camera, const Transform *nodeTransform)>> appliers;

            void setParameter(const std::string &paramName, std::function<std::function<void(const Camera *, const Transform *)>(GLuint, GLint)> getApplier);
        };
    }
}

#endif
