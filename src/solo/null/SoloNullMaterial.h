/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloMaterial.h"

namespace solo
{
    class NullMaterial final : public Material
    {
    public:
        NullMaterial() {}

        auto getEffect() const -> sptr<Effect> override final { return nullptr; }

        void setFloatParameter(const str& name, float value) override final {}
        void setVector2Parameter(const str& name, const Vector2& value) override final {}
        void setVector3Parameter(const str& name, const Vector3& value) override final {}
        void setVector4Parameter(const str& name, const Vector4& value) override final {}
        void setMatrixParameter(const str& name, const Matrix& value) override final {}
        void setTextureParameter(const str& name, sptr<Texture> value) override final {}

        void bindParameter(const str &name, ParameterBinding binding) override final {}
    };
}