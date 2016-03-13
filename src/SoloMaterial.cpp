#include "SoloMaterial.h"
#include "SoloRenderContext.h"
#include "SoloTransform.h"
#include "SoloCamera.h"
#include "SoloTexture.h"

using namespace solo;


Material::Material(Renderer* renderer, shared<Effect> effect):
    renderer(renderer),
    effect(effect)
{
}


Material::~Material()
{
    for (const auto& p: parameters)
        renderer->destroyUniform(p.second.handle);
}


Material::ParameterData& Material::initParameter(const std::string& name, ParameterValueType type,
    UniformType uniformType, uint32_t uniformComponentCount)
{
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), uniformType, uniformComponentCount, effect->getHandle());
        data.type = type;
    }
    return data;
}


void Material::setFloatParameter(const std::string& name, float value)
{
    setParameter(name, ParameterValueType::Float, UniformType::Float, 1, &ParameterData::floatValue, value);
}


void Material::setFloatArrayParameter(const std::string& name, const std::vector<float>& value)
{
    setParameter(name, ParameterValueType::FloatArray, UniformType::FloatArray, static_cast<uint32_t>(value.size()), &ParameterData::floatArrayValue, value);
}


void Material::setVector2Parameter(const std::string& name, const Vector2& value)
{
    setParameter(name, ParameterValueType::Vector2, UniformType::Vector2, 1, &ParameterData::vector2Value, value);
}


void Material::setVector2ArrayParameter(const std::string& name, const std::vector<Vector2>& value)
{
    setParameter(name, ParameterValueType::Vector2Array, UniformType::Vector2Array, static_cast<uint32_t>(value.size()), &ParameterData::vector2ArrayValue, value);
}


void Material::setVector3Parameter(const std::string& name, const Vector3& value)
{
    setParameter(name, ParameterValueType::Vector3, UniformType::Vector3, 1, &ParameterData::vector3Value, value);
}


void Material::setVector3ArrayParameter(const std::string& name, const std::vector<Vector3>& value)
{
    setParameter(name, ParameterValueType::Vector3Array, UniformType::Vector3Array, static_cast<uint32_t>(value.size()), &ParameterData::vector3ArrayValue, value);
}


void Material::setVector4Parameter(const std::string& name, const Vector4& value)
{
    setParameter(name, ParameterValueType::Vector4, UniformType::Vector4, 1, &ParameterData::vector4Value, value);
}


void Material::setVector4ArrayParameter(const std::string& name, const std::vector<Vector4>& value)
{
    setParameter(name, ParameterValueType::Vector4Array, UniformType::Vector4Array, static_cast<uint32_t>(value.size()), &ParameterData::vector4ArrayValue, value);
}


void Material::setMatrixParameter(const std::string& name, const Matrix& value)
{
    setParameter(name, ParameterValueType::Matrix, UniformType::Matrix, 1, &ParameterData::matrixValue, value);
}


void Material::setMatrixArrayParameter(const std::string& name, const std::vector<Matrix>& value)
{
    setParameter(name, ParameterValueType::MatrixArray, UniformType::MatrixArray, static_cast<uint32_t>(value.size()), &ParameterData::matrixArrayValue, value);
}


void Material::setTextureParameter(const std::string& name, shared<Texture> value)
{
    setParameter(name, ParameterValueType::Texture, UniformType::Texture, 1, &ParameterData::textureValue, value);
}


void Material::setParameterAutoBinding(const std::string& name, AutoBinding autoBinding)
{
    switch (autoBinding)
    {
        case AutoBinding::WorldMatrix:
            initParameter(name, ParameterValueType::WorldMatrix, UniformType::Matrix, 1);
            break;
        case AutoBinding::ViewMatrix:
            initParameter(name, ParameterValueType::ViewMatrix, UniformType::Matrix, 1);
            break;
        case AutoBinding::ProjectionMatrix:
            initParameter(name, ParameterValueType::ProjectionMatrix, UniformType::Matrix, 1);
            break;
        case AutoBinding::WorldViewMatrix:
            initParameter(name, ParameterValueType::WorldViewMatrix, UniformType::Matrix, 1);
            break;
        case AutoBinding::ViewProjectionMatrix:
            initParameter(name, ParameterValueType::ViewProjectionMatrix, UniformType::Matrix, 1);
            break;
        case AutoBinding::WorldViewProjectionMatrix:
            initParameter(name, ParameterValueType::WorldViewProjectionMatrix, UniformType::Matrix, 1);
            break;
        case AutoBinding::InverseTransposedWorldMatrix:
            initParameter(name, ParameterValueType::InverseTransposedWorldMatrix, UniformType::Matrix, 1);
            break;
        case AutoBinding::InverseTransposedWorldViewMatrix:
            initParameter(name, ParameterValueType::InverseTransposedWorldViewMatrix, UniformType::Matrix, 1);
            break;
        case AutoBinding::CameraWorldPosition:
            initParameter(name, ParameterValueType::CameraWorldPosition, UniformType::Vector3, 1);
            break;
        default: break;
    }
}


void Material::bind(RenderContext& context)
{
    applyState();
    
    if (!effect)
        return;

    effect->bind();

    for (const auto& p : parameters)
    {
        const auto& data = p.second;

        switch (data.type)
        {
            case ParameterValueType::Float:
                renderer->setUniform(data.handle, &data.floatValue, 1);
                break;
            case ParameterValueType::FloatArray:
                renderer->setUniform(data.handle, data.floatArrayValue.data(), data.floatArrayValue.size());
                break;
            case ParameterValueType::Vector2:
                renderer->setUniform(data.handle, &data.vector2Value, 1);
                break;
            case ParameterValueType::Vector2Array:
                renderer->setUniform(data.handle, data.vector2ArrayValue.data(), data.vector2ArrayValue.size());
                break;
            case ParameterValueType::Vector3:
                renderer->setUniform(data.handle, &data.vector3Value, 1);
                break;
            case ParameterValueType::Vector3Array:
                renderer->setUniform(data.handle, data.vector3ArrayValue.data(), data.vector3ArrayValue.size());
                break;
            case ParameterValueType::Vector4:
                renderer->setUniform(data.handle, &data.vector4Value, 1);
                break;
            case ParameterValueType::Vector4Array:
                renderer->setUniform(data.handle, data.vector4ArrayValue.data(), data.vector4ArrayValue.size());
                break;
            case ParameterValueType::Matrix:
                renderer->setUniform(data.handle, data.matrixValue.m, 1);
                break;
            case ParameterValueType::MatrixArray:
                renderer->setUniform(data.handle, data.matrixArrayValue.data(), data.matrixArrayValue.size());
                break;
            case ParameterValueType::Texture:
                renderer->setUniform(data.handle, nullptr, 1);
                data.textureValue->bind();
                break;
            case ParameterValueType::WorldMatrix:
                if (context.nodeTransform)
                    renderer->setUniform(data.handle, context.nodeTransform->getWorldMatrix().m, 1);
                break;
            case ParameterValueType::ViewMatrix:
                if (context.camera)
                    renderer->setUniform(data.handle, context.camera->getViewMatrix().m, 1);
                break;
            case ParameterValueType::ProjectionMatrix:
                if (context.camera)
                    renderer->setUniform(data.handle, context.camera->getProjectionMatrix().m, 1);
                break;
            case ParameterValueType::WorldViewMatrix:
                if (context.nodeTransform && context.camera)
                    renderer->setUniform(data.handle, context.nodeTransform->getWorldViewMatrix(context.camera).m, 1);
                break;
            case ParameterValueType::ViewProjectionMatrix:
                if (context.camera)
                    renderer->setUniform(data.handle, context.camera->getViewProjectionMatrix().m, 1);
                break;
            case ParameterValueType::WorldViewProjectionMatrix:
                if (context.nodeTransform && context.camera)
                    renderer->setUniform(data.handle, context.nodeTransform->getWorldViewProjectionMatrix(context.camera).m, 1);
                break;
            case ParameterValueType::InverseTransposedWorldMatrix:
                if (context.nodeTransform)
                    renderer->setUniform(data.handle, context.nodeTransform->getInverseTransposedWorldMatrix().m, 1);
                break;
            case ParameterValueType::InverseTransposedWorldViewMatrix:
                if (context.nodeTransform && context.camera)
                    renderer->setUniform(data.handle, context.nodeTransform->getInverseTransposedWorldViewMatrix(context.camera).m, 1);
                break;
            case ParameterValueType::CameraWorldPosition:
                if (context.cameraTransform)
                {
                    auto pos = context.cameraTransform->getWorldPosition();
                    renderer->setUniform(data.handle, &pos, 1);
                }
                break;
            case ParameterValueType::Unknown:
            default:
                break;
        }
    }
}


void Material::applyState()
{
    int flags = 0;
    
    if (polygonFace != PolygonFace::All)
        flags |= StateFlags::CullFace;
    if (polygonFace == PolygonFace::CCW)
        flags |= StateFlags::FrontFaceCCW;

    if (depthWrite)
        flags |= StateFlags::DepthWrite;
    if (depthTest)
        flags |= StateFlags::DepthTest;

    switch (depthPassFunc)
    {
        case DepthPassFunction::Never: flags |= StateFlags::DepthFuncNever; break;
        case DepthPassFunction::Less: flags |= StateFlags::DepthFuncLess; break;
        case DepthPassFunction::Equal: flags |= StateFlags::DepthFuncEqual; break;
        case DepthPassFunction::LEqual: flags |= StateFlags::DepthFuncLEqual; break;
        case DepthPassFunction::Greater: flags |= StateFlags::DepthFuncGreater; break;
        case DepthPassFunction::NotEqual: flags |= StateFlags::DepthFuncNotEqual; break;
        case DepthPassFunction::GEqual: flags |= StateFlags::DepthFuncGEqual; break;
        case DepthPassFunction::Always: flags |= StateFlags::DepthFuncAlways; break;
        default: break;
    }

    renderer->setState(flags);
}
