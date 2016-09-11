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

#include "SoloMaterialParameter.h"
#include "SoloEffect.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloMatrix.h"
#include "SoloTexture.h"
#include "SoloRenderContext.h"
#include "SoloTransform.h"
#include "SoloCamera.h"
#include <vector>

using namespace solo;


template <class T>
class ValueMaterialParameter final: public MaterialParameter
{
public:
    ValueMaterialParameter(Renderer* renderer, Effect* effect, MaterialParameterType type, const char* name):
        MaterialParameter(renderer, effect, type, name)
    {
    }

    void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const T*>(value);
    }

    void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, &value, 1);
    }

private:
    T value;
};


template <class T>
class ValueArrayMaterialParameter final: public MaterialParameter
{
public:
    ValueArrayMaterialParameter(Renderer* renderer, Effect* effect, MaterialParameterType type, const char* name) :
        MaterialParameter(renderer, effect, type, name) 
    {
    }

    void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const std::vector<T>*>(value);
    }

    void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, value.data(), static_cast<uint32_t>(value.size()));
    }

private:
    std::vector<T> value;
};


class TextureMaterialParameter final : public MaterialParameter
{
public:
    TextureMaterialParameter(Renderer* renderer, Effect* effect, const char* name) :
        MaterialParameter(renderer, effect, MaterialParameterType::Texture, name)
    {
    }

    void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const sptr<Texture>*>(value);
    }

    void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, nullptr, 1);
        value->bind();
    }

private:
    sptr<Texture> value;
};


class AutoBindingMaterialParameter final : public MaterialParameter
{
public:
    AutoBindingMaterialParameter(Renderer* renderer, Effect* effect, MaterialParameterType type, const char* name) :
        MaterialParameter(renderer, effect, type, name)
    {
    }

    void setValue(const void* value) override final
    {
    }

    void apply(const RenderContext& context) override final
    {
        switch (type)
        {
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
};


auto MaterialParameter::create(Renderer* renderer, Effect* effect, MaterialParameterType type, const char* name) -> sptr<MaterialParameter>
{
    switch (type)
    {
        case MaterialParameterType::Float:
            return std::make_shared<ValueMaterialParameter<float>>(renderer, effect, MaterialParameterType::Float, name);
        case MaterialParameterType::FloatArray:
            return std::make_shared<ValueArrayMaterialParameter<float>>(renderer, effect, MaterialParameterType::FloatArray, name);

        case MaterialParameterType::Vector2:
            return std::make_shared<ValueMaterialParameter<Vector2>>(renderer, effect, MaterialParameterType::Vector2, name);
        case MaterialParameterType::Vector2Array:
            return std::make_shared<ValueArrayMaterialParameter<Vector2>>(renderer, effect, MaterialParameterType::Vector2Array, name);

        case MaterialParameterType::Vector3:
            return std::make_shared<ValueMaterialParameter<Vector3>>(renderer, effect, MaterialParameterType::Vector3, name);
        case MaterialParameterType::Vector3Array:
            return std::make_shared<ValueArrayMaterialParameter<Vector3>>(renderer, effect, MaterialParameterType::Vector3Array, name);

        case MaterialParameterType::Vector4:
            return std::make_shared<ValueMaterialParameter<Vector4>>(renderer, effect, MaterialParameterType::Vector4, name);
        case MaterialParameterType::Vector4Array:
            return std::make_shared<ValueArrayMaterialParameter<Vector4>>(renderer, effect, MaterialParameterType::Vector4Array, name);

        case MaterialParameterType::Matrix:
            return std::make_shared<ValueMaterialParameter<Matrix>>(renderer, effect, MaterialParameterType::Matrix, name);
        case MaterialParameterType::MatrixArray:
            return std::make_shared<ValueArrayMaterialParameter<Matrix>>(renderer, effect, MaterialParameterType::MatrixArray, name);

        case MaterialParameterType::Texture:
            return std::make_shared<TextureMaterialParameter>(renderer, effect, name);

        case MaterialParameterType::WorldMatrix: 
        case MaterialParameterType::ViewMatrix: 
        case MaterialParameterType::ProjectionMatrix: 
        case MaterialParameterType::WorldViewMatrix: 
        case MaterialParameterType::ViewProjectionMatrix: 
        case MaterialParameterType::WorldViewProjectionMatrix: 
        case MaterialParameterType::InverseTransposedWorldMatrix: 
        case MaterialParameterType::InverseTransposedWorldViewMatrix: 
        case MaterialParameterType::CameraWorldPosition:
            return std::make_shared<AutoBindingMaterialParameter>(renderer, effect, type, name);

        default:
            SL_ERR("Unknown material parameter type");
            return nullptr;
    }
}


MaterialParameter::MaterialParameter(Renderer* renderer, Effect* effect, MaterialParameterType type, const char* name):
    type(type),
    renderer(renderer)
{
    handle = renderer->createUniform(name, getUniformType(type), effect->getHandle());
}


MaterialParameter::~MaterialParameter()
{
    renderer->destroyUniform(handle);
}


auto MaterialParameter::getType() const -> MaterialParameterType
{
    return type;
}


auto MaterialParameter::getUniformType(MaterialParameterType type) -> UniformType
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