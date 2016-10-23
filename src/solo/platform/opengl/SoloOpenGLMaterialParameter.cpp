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

#include "SoloOpenGLMaterialParameter.h"
#include "SoloDevice.h"
#include "SoloEffect.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloTexture.h"
#include "SoloMatrix.h"
#include "SoloOpenGLRenderer.h"
#include "SoloRenderContext.h"
#include "SoloTransform.h"
#include "SoloCamera.h"
#include "SoloOpenGLEffect.h"


#ifdef SL_OPENGL_RENDERER

namespace solo
{
    class MaterialParameterValue
    {
    public:
        virtual ~MaterialParameterValue() {}
        virtual void apply(OpenGLRenderer* renderer, UniformHandle handle) = 0;
    };
}


using namespace solo;


template <class T>
class ScalarValue final: public MaterialParameterValue
{
public:
    explicit ScalarValue(const void* value)
    {
        this->value = *reinterpret_cast<const T*>(value);
    }

    void apply(OpenGLRenderer* renderer, UniformHandle handle) override final
    {
        renderer->setUniform(handle, &value, 1);
    }

private:
    T value;
};


template <class T>
class ArrayValue final: public MaterialParameterValue
{
public:
    explicit ArrayValue(const void* value)
    {
        this->value = *reinterpret_cast<const std::vector<T>*>(value);
    }

    void apply(OpenGLRenderer* renderer, UniformHandle handle) override final
    {
        renderer->setUniform(handle, value.data(), static_cast<uint32_t>(value.size()));
    }

private:
    std::vector<T> value;
};


class TextureValue final : public MaterialParameterValue
{
public:
    explicit TextureValue(const void* value)
    {
        this->value = *reinterpret_cast<const sptr<Texture>*>(value);
    }

    void apply(OpenGLRenderer* renderer, UniformHandle handle) override final
    {
        renderer->setUniform(handle, nullptr, 1);
        value->bind();
    }

private:
    sptr<Texture> value;
};


OpenGLMaterialParameter::OpenGLMaterialParameter(Renderer* renderer, Effect* effect, MaterialParameterType type, const char* name):
    MaterialParameter(type)
{
    this->renderer = dynamic_cast<OpenGLRenderer*>(renderer);
    handle = this->renderer->createUniform(name, getUniformType(type), dynamic_cast<OpenGLEffect*>(effect)->getHandle());
}


OpenGLMaterialParameter::~OpenGLMaterialParameter()
{
    renderer->destroyUniform(handle);
}


void OpenGLMaterialParameter::setValue(const void* value)
{
    // TODO reuse value if type is the same
    switch (type)
    {
        case MaterialParameterType::Float:
            this->value = std::make_unique<ScalarValue<float>>(value);
            break;
        case MaterialParameterType::FloatArray:
            this->value = std::make_unique<ArrayValue<float>>(value);
            break;

        case MaterialParameterType::Vector2:
            this->value = std::make_unique<ScalarValue<Vector2>>(value);
            break;
        case MaterialParameterType::Vector2Array:
            this->value = std::make_unique<ArrayValue<Vector2>>(value);
            break;

        case MaterialParameterType::Vector3:
            this->value = std::make_unique<ScalarValue<Vector3>>(value);
            break;
        case MaterialParameterType::Vector3Array:
            this->value = std::make_unique<ArrayValue<Vector3>>(value);
            break;

        case MaterialParameterType::Vector4:
            this->value = std::make_unique<ScalarValue<Vector4>>(value);
            break;
        case MaterialParameterType::Vector4Array:
            this->value = std::make_unique<ArrayValue<Vector4>>(value);
            break;

        case MaterialParameterType::Matrix:
            this->value = std::make_unique<ScalarValue<Matrix>>(value);
            break;
        case MaterialParameterType::MatrixArray:
            this->value = std::make_unique<ArrayValue<Matrix>>(value);
            break;
        
        case MaterialParameterType::Texture:
            this->value = std::make_unique<TextureValue>(value);
            break;

        default:
            break;
    }
}


void OpenGLMaterialParameter::apply(const RenderContext& context)
{
    switch (type)
    {
        case MaterialParameterType::Float:
        case MaterialParameterType::FloatArray:
        case MaterialParameterType::Vector2:
        case MaterialParameterType::Vector2Array:
        case MaterialParameterType::Vector3:
        case MaterialParameterType::Vector3Array:
        case MaterialParameterType::Vector4:
        case MaterialParameterType::Vector4Array:
        case MaterialParameterType::Matrix:
        case MaterialParameterType::MatrixArray:
        case MaterialParameterType::Texture:
            if (value)
                value->apply(renderer, handle);
            break;

        case MaterialParameterType::WorldMatrix:
            if (context.nodeTransform)
                renderer->setUniform(handle, context.nodeTransform->getWorldMatrix().m, 1);
            break;
        case MaterialParameterType::ViewMatrix:
            if (context.camera)
                renderer->setUniform(handle, context.camera->getViewMatrix().m, 1);
            break;
        case MaterialParameterType::ProjectionMatrix:
            if (context.camera)
                renderer->setUniform(handle, context.camera->getProjectionMatrix().m, 1);
            break;
        case MaterialParameterType::WorldViewMatrix:
            if (context.nodeTransform && context.camera)
                renderer->setUniform(handle, context.nodeTransform->getWorldViewMatrix(context.camera).m, 1);
            break;
        case MaterialParameterType::ViewProjectionMatrix:
            if (context.camera)
                renderer->setUniform(handle, context.camera->getViewProjectionMatrix().m, 1);
            break;
        case MaterialParameterType::WorldViewProjectionMatrix:
            if (context.nodeTransform && context.camera)
                renderer->setUniform(handle, context.nodeTransform->getWorldViewProjectionMatrix(context.camera).m, 1);
            break;
        case MaterialParameterType::InverseTransposedWorldMatrix:
            if (context.nodeTransform)
                renderer->setUniform(handle, context.nodeTransform->getInvTransposedWorldMatrix().m, 1);
            break;
        case MaterialParameterType::InverseTransposedWorldViewMatrix:
            if (context.nodeTransform && context.camera)
                renderer->setUniform(handle, context.nodeTransform->getInvTransposedWorldViewMatrix(context.camera).m, 1);
            break;
        case MaterialParameterType::CameraWorldPosition:
            if (context.cameraTransform)
            {
                auto pos = context.cameraTransform->getWorldPosition();
                renderer->setUniform(handle, &pos, 1);
            }
            break;
        default:
            break;
    }
}


auto OpenGLMaterialParameter::getUniformType(MaterialParameterType type) -> UniformType
{
    switch (type)
    {
        case MaterialParameterType::Float: return UniformType::Float;
        case MaterialParameterType::FloatArray: return UniformType::FloatArray;
        case MaterialParameterType::Vector2: return UniformType::Vector2;
        case MaterialParameterType::Vector2Array: return UniformType::Vector2Array;
        case MaterialParameterType::Vector3: return UniformType::Vector3;
        case MaterialParameterType::Vector3Array: return UniformType::Vector3Array;
        case MaterialParameterType::Vector4: return UniformType::Vector4;
        case MaterialParameterType::Vector4Array: return UniformType::Vector4Array;
        case MaterialParameterType::Matrix: return UniformType::Matrix;
        case MaterialParameterType::MatrixArray: return UniformType::MatrixArray;
        case MaterialParameterType::Texture: return UniformType::Texture;
        case MaterialParameterType::WorldMatrix: return UniformType::TextureArray;
        case MaterialParameterType::CameraWorldPosition: return UniformType::Vector3;
        case MaterialParameterType::ViewMatrix:
        case MaterialParameterType::ProjectionMatrix:
        case MaterialParameterType::WorldViewMatrix:
        case MaterialParameterType::ViewProjectionMatrix:
        case MaterialParameterType::WorldViewProjectionMatrix:
        case MaterialParameterType::InverseTransposedWorldMatrix:
        case MaterialParameterType::InverseTransposedWorldViewMatrix:
            return UniformType::Matrix;
        default:
            SL_ERR("Unknown material parameter type");
            return UniformType::Float;
    }
}

#else
#   error OpenGL renderer is not supported on this platform
#endif
