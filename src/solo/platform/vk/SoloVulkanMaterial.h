/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloMaterial.h"

namespace solo
{
    class Device;
    class Effect;
    class Camera;

    namespace vk
    {
        class Renderer;

        class Material final: public solo::Material
        {
        public:
            Material(Device *device, sptr<solo::Effect> effect);
            ~Material();

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
            
            void setTextureParameter(const std::string &name, sptr<Texture> value) override final;
            
            void bindWorldMatrixParameter(const std::string &name) override final;
            void bindViewMatrixParameter(const std::string &name) override final;
            void bindProjectionMatrixParameter(const std::string &name) override final;
            void bindWorldViewMatrixParameter(const std::string &name) override final;
            void bindViewProjectionMatrixParameter(const std::string &name) override final;
            void bindWorldViewProjectionMatrixParameter(const std::string &name) override final;
            void bindInvTransposedWorldMatrixParameter(const std::string &name) override final;
            void bindInvTransposedWorldViewMatrixParameter(const std::string &name) override final;
            void bindCameraWorldPositionParameter(const std::string &name) override final;
        
        private:
            Renderer *renderer = nullptr;
        };
    }
}

#endif