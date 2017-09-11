/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloMaterial.h"

namespace solo
{
    namespace null
    {
        class Material final : public solo::Material
        {
        public:
            Material() {}

            auto getEffect() const -> solo::Effect* override final { return nullptr; }

            void setFloatParameter(const std::string& name, float value) override final {}
            void setVector2Parameter(const std::string& name, const Vector2& value) override final {}
            void setVector3Parameter(const std::string& name, const Vector3& value) override final {}
            void setVector4Parameter(const std::string& name, const Vector4& value) override final {}
            void setMatrixParameter(const std::string& name, const Matrix& value) override final {}
            void setTextureParameter(const std::string& name, sptr<Texture> value) override final {}

            void bindParameter(const std::string &name, BindParameterSemantics semantics) override final {}
        };
    }
}