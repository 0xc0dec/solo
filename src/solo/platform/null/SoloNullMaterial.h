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

#include "SoloMaterial.h"

namespace solo
{
    namespace null
    {
        class Material final : public solo::Material
        {
        public:
            explicit Material(Device *device, const sptr<Effect> &effect) :
                solo::Material(device, effect)
            {
            }

            void setFloatParameter(const std::string& name, float value) override final {}
            void setFloatArrayParameter(const std::string& name, const std::vector<float>& value) override final {}
            void setVector2Parameter(const std::string& name, const Vector2& value) override final {}
            void setVector2ArrayParameter(const std::string& name, const std::vector<Vector2>& value) override final {}
            void setVector3Parameter(const std::string& name, const Vector3& value) override final {}
            void setVector3ArrayParameter(const std::string& name, const std::vector<Vector3>& value) override final {}
            void setVector4Parameter(const std::string& name, const Vector4& value) override final {}
            void setVector4ArrayParameter(const std::string& name, const std::vector<Vector4>& value) override final {}
            void setMatrixParameter(const std::string& name, const Matrix& value) override final {}
            void setMatrixArrayParameter(const std::string& name, const std::vector<Matrix>& value) override final {}
            void setTextureParameter(const std::string& name, sptr<Texture> value) override final {}
            void bindWorldMatrixParameter(const std::string& name) override final {}
            void bindViewMatrixParameter(const std::string& name) override final {}
            void bindProjectionMatrixParameter(const std::string& name) override final {}
            void bindWorldViewMatrixParameter(const std::string& name) override final {}
            void bindViewProjectionMatrixParameter(const std::string& name) override final {}
            void bindWorldViewProjectionMatrixParameter(const std::string& name) override final {}
            void bindInvTransposedWorldMatrixParameter(const std::string& name) override final {}
            void bindInvTransposedWorldViewMatrixParameter(const std::string& name) override final {}
            void bindCameraWorldPositionParameter(const std::string& name) override final {}

        protected:
            void applyState() override final
            {
            }

            void applyParams(const Camera* camera, Transform* nodeTransform) override final
            {
            }
        };
    }
}