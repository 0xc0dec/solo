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


template <class T, MaterialParameterType Type>
class ValueMaterialParameter final: public MaterialParameter
{
public:
    ValueMaterialParameter(Renderer* renderer, Effect* effect, const char* name):
        MaterialParameter(renderer, effect, Type, name)
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const T*>(value);
    }

    virtual void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, &value, 1);
    }

private:
    T value;
};


template <class T, MaterialParameterType Type>
class ValueArrayMaterialParameter final: public MaterialParameter
{
public:
    ValueArrayMaterialParameter(Renderer* renderer, Effect* effect, const char* name) :
        MaterialParameter(renderer, effect, Type, name) 
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const std::vector<T>*>(value);
    }

    virtual void apply(const RenderContext& context) override final
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

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const sptr<Texture>*>(value);
    }

    virtual void apply(const RenderContext& context) override final
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

    virtual void setValue(const void* value) override final
    {
    }

    virtual void apply(const RenderContext& context) override final
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
                    renderer->setUniform(handle, context.nodeTransform->getInverseTransposedWorldMatrix().m, 1);
                break;
            case MaterialParameterType::InverseTransposedWorldViewMatrix:
                if (context.nodeTransform && context.camera)
                    renderer->setUniform(handle, context.nodeTransform->getInverseTransposedWorldViewMatrix(context.camera).m, 1);
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


auto MaterialParameter::create(Renderer* renderer, Effect* effect, MaterialParameterType type, const char* name) -> uptr<MaterialParameter>
{
    switch (type)
    {
        case MaterialParameterType::Float:
            return std::make_unique<ValueMaterialParameter<float, MaterialParameterType::Float>>(renderer, effect, name);
        case MaterialParameterType::FloatArray:
            return std::make_unique<ValueArrayMaterialParameter<float, MaterialParameterType::FloatArray>>(renderer, effect, name);

        case MaterialParameterType::Vector2:
            return std::make_unique<ValueMaterialParameter<Vector2, MaterialParameterType::Vector2>>(renderer, effect, name);
        case MaterialParameterType::Vector2Array:
            return std::make_unique<ValueArrayMaterialParameter<Vector2, MaterialParameterType::Vector2Array>>(renderer, effect, name);

        case MaterialParameterType::Vector3:
            return std::make_unique<ValueMaterialParameter<Vector3, MaterialParameterType::Vector3>>(renderer, effect, name);
        case MaterialParameterType::Vector3Array:
            return std::make_unique<ValueArrayMaterialParameter<Vector3, MaterialParameterType::Vector3Array>>(renderer, effect, name);

        case MaterialParameterType::Vector4:
            return std::make_unique<ValueMaterialParameter<Vector4, MaterialParameterType::Vector4>>(renderer, effect, name);
        case MaterialParameterType::Vector4Array:
            return std::make_unique<ValueArrayMaterialParameter<Vector4, MaterialParameterType::Vector4Array>>(renderer, effect, name);

        case MaterialParameterType::Matrix:
            return std::make_unique<ValueMaterialParameter<Matrix, MaterialParameterType::Matrix>>(renderer, effect, name);
        case MaterialParameterType::MatrixArray:
            return std::make_unique<ValueArrayMaterialParameter<Matrix, MaterialParameterType::MatrixArray>>(renderer, effect, name);

        case MaterialParameterType::Texture:
            return std::make_unique<TextureMaterialParameter>(renderer, effect, name);

        case MaterialParameterType::WorldMatrix: 
        case MaterialParameterType::ViewMatrix: 
        case MaterialParameterType::ProjectionMatrix: 
        case MaterialParameterType::WorldViewMatrix: 
        case MaterialParameterType::ViewProjectionMatrix: 
        case MaterialParameterType::WorldViewProjectionMatrix: 
        case MaterialParameterType::InverseTransposedWorldMatrix: 
        case MaterialParameterType::InverseTransposedWorldViewMatrix: 
        case MaterialParameterType::CameraWorldPosition:
            return std::make_unique<AutoBindingMaterialParameter>(renderer, effect, type, name);

        default:
            SL_THROW(InvalidInputException, "Unknown material parameter type");
    }
}


MaterialParameter::~MaterialParameter()
{
    renderer->destroyUniform(handle);
}


auto MaterialParameter::getType() const -> MaterialParameterType
{
    return type;
}


MaterialParameter::MaterialParameter(Renderer* renderer, Effect* effect, MaterialParameterType type, const char* name):
    type(type),
    renderer(renderer)
{
    handle = renderer->createUniform(name, getUniformType(type), effect->getHandle());
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
            SL_THROW(InvalidInputException, "Material type parameter not convertible to uniform type");
    }
}
