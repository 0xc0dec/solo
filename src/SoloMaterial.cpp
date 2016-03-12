#include "SoloMaterial.h"
#include "SoloEffect.h"
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
    // TODO destroy stuff
}


void Material::setFloatParameter(const std::string& name, float value)
{
    // TODO extract method
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), UniformType::Float, 1, effect->getHandle());
        data.type = ParameterValueType::Float;
    }
    data.floatValue = value;
}


void Material::setFloatArrayParameter(const std::string& name, const std::vector<float>& value)
{
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), UniformType::FloatArray, value.size(), effect->getHandle());
        data.type = ParameterValueType::FloatArray;
    }
    data.floatArrayValue = value;
}


void Material::setVector2Parameter(const std::string& name, const Vector2& value)
{
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), UniformType::Vector2, 1, effect->getHandle());
        data.type = ParameterValueType::Vector2;
    }
    data.vector2Value = value;
}


void Material::setVector2ArrayParameter(const std::string& name, const std::vector<Vector2>& value)
{
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), UniformType::Vector2Array, value.size(), effect->getHandle());
        data.type = ParameterValueType::Vector2Array;
    }
    data.vector2ArrayValue = value;
}


void Material::setVector3Parameter(const std::string& name, const Vector3& value)
{
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), UniformType::Vector3, 1, effect->getHandle());
        data.type = ParameterValueType::Vector3;
    }
    data.vector3Value = value;
}


void Material::setVector3ArrayParameter(const std::string& name, const std::vector<Vector3>& value)
{
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), UniformType::Vector3Array, value.size(), effect->getHandle());
        data.type = ParameterValueType::Vector3Array;
    }
    data.vector3ArrayValue = value;
}


void Material::setVector4Parameter(const std::string& name, const Vector4& value)
{
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), UniformType::Vector4, 1, effect->getHandle());
        data.type = ParameterValueType::Vector4;
    }
    data.vector4Value = value;
}


void Material::setVector4ArrayParameter(const std::string& name, const std::vector<Vector4>& value)
{
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), UniformType::Vector4Array, value.size(), effect->getHandle());
        data.type = ParameterValueType::Vector4Array;
    }
    data.vector4ArrayValue = value;
}


void Material::setMatrixParameter(const std::string& name, const Matrix& value)
{
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), UniformType::Matrix, 1, effect->getHandle());
        data.type = ParameterValueType::Matrix;
    }
    data.matrixValue = value;
}


void Material::setMatrixArrayParameter(const std::string& name, const std::vector<Matrix>& value)
{
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), UniformType::MatrixArray, value.size(), effect->getHandle());
        data.type = ParameterValueType::MatrixArray;
    }
    data.matrixArrayValue = value;
}


void Material::setTextureParameter(const std::string& name, shared<Texture> value)
{
    auto& data = parameters[name];
    if (data.type == ParameterValueType::Unknown)
    {
        data.handle = renderer->createUniform(name.c_str(), UniformType::Texture, 1, effect->getHandle());
        data.type = ParameterValueType::Texture;
    }
    data.textureValue = value;
}


void Material::setParameterAutoBinding(const std::string& name, AutoBinding autoBinding)
{
    auto& data = parameters[name];
    // TODO need moar beautiful here
    switch (autoBinding)
    {
        case AutoBinding::WorldMatrix:
            if (data.type == ParameterValueType::Unknown)
                data.handle = renderer->createUniform(name.c_str(), UniformType::Matrix, 1, effect->getHandle());
            data.type = ParameterValueType::WorldMatrix;
            break;
        case AutoBinding::ViewMatrix:
            if (data.type == ParameterValueType::Unknown)
                data.handle = renderer->createUniform(name.c_str(), UniformType::Matrix, 1, effect->getHandle());
            data.type = ParameterValueType::ViewMatrix;
            break;
        case AutoBinding::ProjectionMatrix:
            if (data.type == ParameterValueType::Unknown)
                data.handle = renderer->createUniform(name.c_str(), UniformType::Matrix, 1, effect->getHandle());
            data.type = ParameterValueType::ProjectionMatrix;
            break;
        case AutoBinding::WorldViewMatrix:
            if (data.type == ParameterValueType::Unknown)
                data.handle = renderer->createUniform(name.c_str(), UniformType::Matrix, 1, effect->getHandle());
            data.type = ParameterValueType::WorldViewMatrix;
            break;
        case AutoBinding::ViewProjectionMatrix:
            if (data.type == ParameterValueType::Unknown)
                data.handle = renderer->createUniform(name.c_str(), UniformType::Matrix, 1, effect->getHandle());
            data.type = ParameterValueType::ViewProjectionMatrix;
            break;
        case AutoBinding::WorldViewProjectionMatrix:
            if (data.type == ParameterValueType::Unknown)
                data.handle = renderer->createUniform(name.c_str(), UniformType::Matrix, 1, effect->getHandle());
            data.type = ParameterValueType::WorldViewProjectionMatrix;
            break;
        case AutoBinding::InverseTransposedWorldMatrix:
            if (data.type == ParameterValueType::Unknown)
                data.handle = renderer->createUniform(name.c_str(), UniformType::Matrix, 1, effect->getHandle());
            data.type = ParameterValueType::InverseTransposedWorldMatrix;
            break;
        case AutoBinding::InverseTransposedWorldViewMatrix:
            if (data.type == ParameterValueType::Unknown)
                data.handle = renderer->createUniform(name.c_str(), UniformType::Matrix, 1, effect->getHandle());
            data.type = ParameterValueType::InverseTransposedWorldViewMatrix;
            break;
        case AutoBinding::CameraWorldPosition:
            if (data.type == ParameterValueType::Unknown)
                data.handle = renderer->createUniform(name.c_str(), UniformType::Vector3, 1, effect->getHandle());
            data.type = ParameterValueType::CameraWorldPosition;
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
                data.textureValue->apply();
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


void Material::unbind(RenderContext& context)
{
    if (effect)
        effect->unbind();
}


void Material::applyState()
{
    int flags = 0;
    
    if (polygonFace != PolygonFace::All)
        flags |= CullFace;
    if (polygonFace == PolygonFace::CCW)
        flags |= FrontFaceCCW;

    if (depthWrite)
        flags |= DepthWrite;
    if (depthTest)
        flags |= DepthTest;

    switch (depthPassFunc)
    {
        case DepthPassFunction::Never: flags |= DepthFuncNever; break;
        case DepthPassFunction::Less: flags |= DepthFuncLess; break;
        case DepthPassFunction::Equal: flags |= DepthFuncEqual; break;
        case DepthPassFunction::LEqual: flags |= DepthFuncLEqual; break;
        case DepthPassFunction::Greater: flags |= DepthFuncGreater; break;
        case DepthPassFunction::NotEqual: flags |= DepthFuncNotEqual; break;
        case DepthPassFunction::GEqual: flags |= DepthFuncGEqual; break;
        case DepthPassFunction::Always: flags |= DepthFuncAlways; break;
        default: break;
    }

    renderer->setState(flags);
}
