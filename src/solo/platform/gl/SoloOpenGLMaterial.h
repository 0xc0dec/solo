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
#include <unordered_set>

namespace solo
{
    class Device;
    class Camera;

    namespace gl
    {
        class Renderer;
        class Effect;
        class Texture;

        class Material final : public solo::Material
        {
        public:
            Material(sptr<solo::Effect> effect);
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
            enum class UniformType
            {
                Float,
                FloatArray,
                Vector2,
                Vector2Array,
                Vector3,
                Vector3Array,
                Vector4,
                Vector4Array,
                Matrix,
                MatrixArray,
                Texture,
                TextureArray,
            };

            template <class T>
            using StrKeyMap = std::unordered_map<std::string, T>;
            using StrSet = std::unordered_set<std::string>;

            sptr<Effect> effect = nullptr;

            StrKeyMap<GLint> uniformLocations;
            StrKeyMap<GLint> uniformIndexes;
            StrKeyMap<UniformType> uniformTypes;

            StrKeyMap<float> floatParams;
            StrKeyMap<Vector2> vector2Params;
            StrKeyMap<Vector3> vector3Params;
            StrKeyMap<Vector4> vector4Params;
            StrKeyMap<Matrix> matrixParams;
            StrKeyMap<std::vector<float>> floatArrayParams;
            StrKeyMap<std::vector<Vector2>> vector2ArrayParams;
            StrKeyMap<std::vector<Vector3>> vector3ArrayParams;
            StrKeyMap<std::vector<Vector4>> vector4ArrayParams;
            StrKeyMap<std::vector<Matrix>> matrixArrayParams;
            StrKeyMap<sptr<Texture>> textureParams;

            StrSet worldMatrixParams;
            StrSet viewMatrixParams;
            StrSet projMatrixParams;
            StrSet worldViewMatrixParams;
            StrSet viewProjMatrixParams;
            StrSet worldViewProjMatrixParams;
            StrSet invTransWorldMatrixParams;
            StrSet invTransWorldViewMatrixParams;
            StrSet camWorldPosParams;

            template <class T>
            void applyScalarParams(const StrKeyMap<T> &params) const;

            template <class T>
            void applyVectorParams(const StrKeyMap<T> &params) const;

            template <class T>
            void setParam(StrKeyMap<T> &params, const std::string &name, UniformType uniformType, T value);

            void setAutoBindParam(StrSet &params, const std::string &name, UniformType uniformType);

            void initUniform(const std::string &name, UniformType type);
            void setUniform(const std::string &name, const void *value, uint32_t count) const;
        };

        template <class T>
        void Material::applyScalarParams(const StrKeyMap<T> &params) const
        {
            for (const auto &p : params)
                setUniform(p.first, &p.second, 1);
        }

        template <class T>
        void Material::applyVectorParams(const StrKeyMap<T> &params) const
        {
            for (const auto &p : params)
                setUniform(p.first, p.second.data(), static_cast<uint32_t>(p.second.size()));
        }

        template <class T>
        void Material::setParam(StrKeyMap<T> &params, const std::string &name, UniformType uniformType, T value)
        {
            if (params.find(name) == params.end())
                initUniform(name, uniformType);
            params[name] = value;
        }
    }
}

#endif
