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


class FloatMaterialParameter final: public MaterialParameter
{
public:
    FloatMaterialParameter(Renderer* renderer, Effect* effect, const char* name):
        MaterialParameter(renderer, effect, MaterialParameterType::Float, name)
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const float*>(value);
    }

    virtual void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, &value, 1);
    }

private:
    float value = 0; // TODO store as void*
};


class FloatArrayMaterialParameter final : public MaterialParameter
{
public:
    FloatArrayMaterialParameter(Renderer* renderer, Effect* effect, const char* name):
        MaterialParameter(renderer, effect, MaterialParameterType::FloatArray, name)
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const std::vector<float>*>(value);
    }

    virtual void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, value.data(), value.size());
    }

private:
    std::vector<float> value;
};


class Vector2MaterialParameter final : public MaterialParameter
{
public:
    Vector2MaterialParameter(Renderer* renderer, Effect* effect, const char* name):
        MaterialParameter(renderer, effect, MaterialParameterType::Vector2, name)
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const Vector2*>(value);
    }

    virtual void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, &value, 1);
    }

private:
    Vector2 value;
};


class Vector2ArrayMaterialParameter final : public MaterialParameter
{
public:
    Vector2ArrayMaterialParameter(Renderer* renderer, Effect* effect, const char* name):
        MaterialParameter(renderer, effect, MaterialParameterType::Vector2Array, name)
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const std::vector<Vector2>*>(value);
    }

    virtual void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, &value, value.size());
    }

private:
    std::vector<Vector2> value;
};


class Vector3MaterialParameter final : public MaterialParameter
{
public:
    Vector3MaterialParameter(Renderer* renderer, Effect* effect, const char* name):
        MaterialParameter(renderer, effect, MaterialParameterType::Vector3, name)
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const Vector3*>(value);
    }

    virtual void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, &value, 1);
    }

private:
    Vector3 value;
};


class Vector3ArrayMaterialParameter final : public MaterialParameter
{
public:
    Vector3ArrayMaterialParameter(Renderer* renderer, Effect* effect, const char* name):
        MaterialParameter(renderer, effect, MaterialParameterType::Vector3Array, name)
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const std::vector<Vector3>*>(value);
    }

    virtual void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, &value, value.size());
    }

private:
    std::vector<Vector3> value;
};


class Vector4MaterialParameter final : public MaterialParameter
{
public:
    Vector4MaterialParameter(Renderer* renderer, Effect* effect, const char* name):
        MaterialParameter(renderer, effect, MaterialParameterType::Vector4, name)
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const Vector4*>(value);
    }

    virtual void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, &value, 1);
    }

private:
    Vector4 value;
};


class Vector4ArrayMaterialParameter final : public MaterialParameter
{
public:
    Vector4ArrayMaterialParameter(Renderer* renderer, Effect* effect, const char* name):
        MaterialParameter(renderer, effect, MaterialParameterType::Vector4Array, name)
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const std::vector<Vector4>*>(value);
    }

    virtual void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, &value, value.size());
    }

private:
    std::vector<Vector4> value;
};


class MatrixMaterialParameter final : public MaterialParameter
{
public:
    MatrixMaterialParameter(Renderer* renderer, Effect* effect, const char* name):
        MaterialParameter(renderer, effect, MaterialParameterType::Matrix, name)
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const Matrix*>(value);
    }

    virtual void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, &value, 1);
    }

private:
    Matrix value;
};


class MatrixArrayMaterialParameter final : public MaterialParameter
{
public:
    MatrixArrayMaterialParameter(Renderer* renderer, Effect* effect, const char* name):
        MaterialParameter(renderer, effect, MaterialParameterType::MatrixArray, name)
    {
    }

    virtual void setValue(const void* value) override final
    {
        this->value = *reinterpret_cast<const std::vector<Matrix>*>(value);
    }

    virtual void apply(const RenderContext& context) override final
    {
        renderer->setUniform(handle, &value, value.size());
    }

private:
    std::vector<Matrix> value;
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
        case MaterialParameterType::Float:        return std::make_unique<FloatMaterialParameter>(renderer, effect, name);
        case MaterialParameterType::FloatArray:   return std::make_unique<FloatArrayMaterialParameter>(renderer, effect, name);
        case MaterialParameterType::Vector2:      return std::make_unique<Vector2MaterialParameter>(renderer, effect, name);
        case MaterialParameterType::Vector2Array: return std::make_unique<Vector2ArrayMaterialParameter>(renderer, effect, name);
        case MaterialParameterType::Vector3:      return std::make_unique<Vector3MaterialParameter>(renderer, effect, name);
        case MaterialParameterType::Vector3Array: return std::make_unique<Vector3ArrayMaterialParameter>(renderer, effect, name);
        case MaterialParameterType::Vector4:      return std::make_unique<Vector4MaterialParameter>(renderer, effect, name);
        case MaterialParameterType::Vector4Array: return std::make_unique<Vector4ArrayMaterialParameter>(renderer, effect, name);
        case MaterialParameterType::Matrix:       return std::make_unique<MatrixMaterialParameter>(renderer, effect, name);
        case MaterialParameterType::MatrixArray:  return std::make_unique<MatrixArrayMaterialParameter>(renderer, effect, name);
        case MaterialParameterType::Texture:      return std::make_unique<TextureMaterialParameter>(renderer, effect, name);
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
    handle = renderer->createUniform(name, getUniformType(type), 0 /* TODO */, effect->getHandle());
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
