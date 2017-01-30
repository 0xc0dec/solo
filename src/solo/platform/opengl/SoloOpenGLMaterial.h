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

#ifdef SL_OPENGL_RENDERER

#include "SoloMaterial.h"
#include <unordered_map>
#include <unordered_set>

namespace solo
{
    class OpenGLRenderer;
    class OpenGLEffect;
    class OpenGLTexture;
    class Device;

    class OpenGLMaterial final: public Material
    {
    public:
        explicit OpenGLMaterial(Device *device, sptr<Effect> effect);

        void setFloatParameter2(const std::string &name, float value);
        void setFloatArrayParameter2(const std::string &name, const std::vector<float> &value);

        void setVector2Parameter2(const std::string &name, const Vector2 &value);
        void setVector2ArrayParameter2(const std::string &name, const std::vector<Vector2> &value);

        void setVector3Parameter2(const std::string &name, const Vector3 &value);
        void setVector3ArrayParameter2(const std::string &name, const std::vector<Vector3> &value);

        void setVector4Parameter2(const std::string &name, const Vector4 &value);
        void setVector4ArrayParameter2(const std::string &name, const std::vector<Vector4> &value);

        void setMatrixParameter2(const std::string &name, const Matrix &value);
        void setMatrixArrayParameter2(const std::string &name, const std::vector<Matrix> &value);

        void setTextureParameter2(const std::string &name, sptr<Texture> value);

        void bindWorldMatrixParameter2(const std::string &name);
        void bindViewMatrixParameter2(const std::string &name);
        void bindProjectionMatrixParameter2(const std::string &name);
        void bindWorldViewMatrixParameter2(const std::string &name);
        void bindViewProjectionMatrixParameter2(const std::string &name);
        void bindWorldViewProjectionMatrixParameter2(const std::string &name);
        void bindInvTransposedWorldMatrixParameter2(const std::string &name);
        void bindInvTransposedWorldViewMatrixParameter2(const std::string &name);
        void bindCameraWorldPositionParameter2(const std::string &name);

    protected:
        void applyState() override final;
        void applyParams();

        OpenGLRenderer *renderer = nullptr;
        sptr<OpenGLEffect> effect = nullptr;

        template <class T>
        using map = std::unordered_map<std::string, T>;
        using strSet = std::unordered_set<std::string>;

        map<uint32_t> uniformHandles;

        map<float> floatParams;
        map<Vector2> vector2Params;
        map<Vector3> vector3Params;
        map<Vector4> vector4Params;
        map<Matrix> matrixParams;
        map<std::vector<float>> floatArrayParams;
        map<std::vector<Vector2>> vector2ArrayParams;
        map<std::vector<Vector3>> vector3ArrayParams;
        map<std::vector<Vector4>> vector4ArrayParams;
        map<std::vector<Matrix>> matrixArrayParams;
        map<sptr<OpenGLTexture>> textureParams;

        strSet worldMatrixParams;
        strSet viewMatrixParams;
        strSet projMatrixParams;
        strSet worldViewMatrixParams;
        strSet viewProjMatrixParams;
        strSet worldViewProjMatrixParams;
        strSet invTransWorldMatrixParams;
        strSet invTransWorldViewMatrixParams;
        strSet camWorldPosParams;
    };
}

#endif
