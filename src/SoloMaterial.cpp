#include "SoloMaterial.h"
#include "SoloEffect.h"
#include "SoloRenderContext.h"
#include "SoloTransform.h"
#include "SoloCamera.h"

using namespace solo;


Material::Material(Renderer* renderer, shared<Effect> effect):
    renderer(renderer),
    effect(effect)
{
}


Material::~Material()
{
    // TODO
}


void Material::setFloatParameter(const std::string& name, float value)
{
    clearAndAssignValue<float>(name, &ParameterData::floatValue, value, ParameterValueType::Float);
}


void Material::setFloatArrayParameter(const std::string& name, const std::vector<float>& value)
{
    clearAndAssignValue<std::vector<float>>(name, &ParameterData::floatArrayValue, value, ParameterValueType::FloatArray);
}


void Material::setVector2Parameter(const std::string& name, const Vector2& value)
{
    clearAndAssignValue<Vector2>(name, &ParameterData::vector2Value, value, ParameterValueType::Vector2);
}


void Material::setVector2ArrayParameter(const std::string& name, const std::vector<Vector2>& value)
{
    clearAndAssignValue<std::vector<Vector2>>(name, &ParameterData::vector2ArrayValue, value, ParameterValueType::Vector2Array);
}


void Material::setVector3Parameter(const std::string& name, const Vector3& value)
{
    clearAndAssignValue<Vector3>(name, &ParameterData::vector3Value, value, ParameterValueType::Vector3);
}


void Material::setVector3ArrayParameter(const std::string& name, const std::vector<Vector3>& value)
{
    clearAndAssignValue<std::vector<Vector3>>(name, &ParameterData::vector3ArrayValue, value, ParameterValueType::Vector3Array);
}


void Material::setVector4Parameter(const std::string& name, const Vector4& value)
{
    clearAndAssignValue<Vector4>(name, &ParameterData::vector4Value, value, ParameterValueType::Vector4);
}


void Material::setVector4ArrayParameter(const std::string& name, const std::vector<Vector4>& value)
{
    clearAndAssignValue<std::vector<Vector4>>(name, &ParameterData::vector4ArrayValue, value, ParameterValueType::Vector4Array);
}


void Material::setMatrixParameter(const std::string& name, const Matrix& value)
{
    clearAndAssignValue<Matrix>(name, &ParameterData::matrixValue, value, ParameterValueType::Matrix);
}


void Material::setMatrixArrayParameter(const std::string& name, const std::vector<Matrix>& value)
{
    clearAndAssignValue<std::vector<Matrix>>(name, &ParameterData::matrixArrayValue, value, ParameterValueType::MatrixArray);
}


void Material::setTextureParameter(const std::string& name, shared<Texture> value)
{
    clearAndAssignValue<shared<Texture>>(name, &ParameterData::textureValue, value, ParameterValueType::Matrix);
}


void Material::setParameterAutoBinding(const std::string& name, AutoBinding autoBinding)
{
    clearAndAssignValue<AutoBinding>(name, &ParameterData::autoBinding, autoBinding, ParameterValueType::AutoBinding);
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
                break;
            case ParameterValueType::AutoBinding:
                applyAutoBinding(data, context);
                break;
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


void Material::clearOldValue(ParameterData& data, ParameterValueType newType)
{
    if (newType == data.type)
        return;
    switch (newType)
    {
        case ParameterValueType::FloatArray:
            data.floatArrayValue.clear();
            break;
        case ParameterValueType::Vector2Array:
            data.vector2ArrayValue.clear();
            break;
        case ParameterValueType::Vector3Array:
            data.vector3ArrayValue.clear();
            break;
        case ParameterValueType::Vector4Array:
            data.vector4ArrayValue.clear();
            break;
        case ParameterValueType::MatrixArray:
            data.matrixArrayValue.clear();
            break;
        case ParameterValueType::Texture:
            data.textureValue = nullptr;
            break;
        default:
            break;
    }
    data.type = ParameterValueType::Float;
}


void Material::applyAutoBinding(const ParameterData& data, const RenderContext& context)
{
    switch (data.autoBinding)
    {
        case AutoBinding::WorldMatrix:
            if (context.nodeTransform)
                renderer->setUniform(data.handle, context.nodeTransform->getWorldMatrix().m, 1);
            break;

        case AutoBinding::ViewMatrix:
            if (context.camera)
                renderer->setUniform(data.handle, context.camera->getViewMatrix().m, 1);
            break;
        case AutoBinding::ProjectionMatrix:
            if (context.camera)
                renderer->setUniform(data.handle, context.camera->getProjectionMatrix().m, 1);
            break;
        case AutoBinding::WorldViewMatrix:
            if (context.nodeTransform && context.camera)
                renderer->setUniform(data.handle, context.nodeTransform->getWorldViewMatrix(context.camera).m, 1);
            break;
        case AutoBinding::ViewProjectionMatrix:
            if (context.camera)
                renderer->setUniform(data.handle, context.camera->getViewProjectionMatrix().m, 1);
            break;
        case AutoBinding::WorldViewProjectionMatrix:
            if (context.nodeTransform && context.camera)
                renderer->setUniform(data.handle, context.nodeTransform->getWorldViewProjectionMatrix(context.camera).m, 1);
            break;
        case AutoBinding::InverseTransposedWorldMatrix:
            if (context.nodeTransform)
                renderer->setUniform(data.handle, context.nodeTransform->getInverseTransposedWorldMatrix().m, 1);
            break;
        case AutoBinding::InverseTransposedWorldViewMatrix:
            if (context.nodeTransform && context.camera)
                renderer->setUniform(data.handle, context.nodeTransform->getInverseTransposedWorldViewMatrix(context.camera).m, 1);
            break;
        case AutoBinding::CameraWorldPosition:
            if (context.cameraTransform)
            {
                auto pos = context.cameraTransform->getWorldPosition();
                renderer->setUniform(data.handle, &pos, 1);
            }
            break;
        default: break;
    }
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
