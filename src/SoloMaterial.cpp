#include "SoloMaterial.h"
#include "SoloRenderContext.h"
#include "SoloTexture.h"
#include "SoloDevice.h"
#include "SoloMaterialParameter.h"

using namespace solo;


auto Material::create(sptr<Effect> effect) -> sptr<Material>
{
    return std::shared_ptr<Material>(new Material(effect));
}


Material::Material(sptr<Effect> effect):
    renderer(Device::get()->getRenderer()),
    effect(effect)
{
}


void Material::setParameter(const std::string& name, MaterialParameterType type, const void* value)
{
    auto where = parameters.find(name);
    if (where == parameters.end() || where->second->getType() != type)
        parameters[name] = MaterialParameter::create(renderer, effect.get(), type, name.c_str());
    parameters.at(name)->setValue(value);
}


void Material::setFloatParameter(const std::string& name, float value)
{
    setParameter(name, MaterialParameterType::Float, &value);
}


void Material::setFloatArrayParameter(const std::string& name, const std::vector<float>& value)
{
    setParameter(name, MaterialParameterType::FloatArray, &value);
}


void Material::setVector2Parameter(const std::string& name, const Vector2& value)
{
    setParameter(name, MaterialParameterType::Vector2, &value);
}


void Material::setVector2ArrayParameter(const std::string& name, const std::vector<Vector2>& value)
{
    setParameter(name, MaterialParameterType::Vector2Array, &value);
}


void Material::setVector3Parameter(const std::string& name, const Vector3& value)
{
    setParameter(name, MaterialParameterType::Vector3, &value);
}


void Material::setVector3ArrayParameter(const std::string& name, const std::vector<Vector3>& value)
{
    setParameter(name, MaterialParameterType::Vector3Array, &value);
}


void Material::setVector4Parameter(const std::string& name, const Vector4& value)
{
    setParameter(name, MaterialParameterType::Vector4, &value);
}


void Material::setVector4ArrayParameter(const std::string& name, const std::vector<Vector4>& value)
{
    setParameter(name, MaterialParameterType::Vector4Array, &value);
}


void Material::setMatrixParameter(const std::string& name, const Matrix& value)
{
    setParameter(name, MaterialParameterType::Matrix, &value);
}


void Material::setMatrixArrayParameter(const std::string& name, const std::vector<Matrix>& value)
{
    setParameter(name, MaterialParameterType::MatrixArray, &value);
}


void Material::setTextureParameter(const std::string& name, sptr<Texture> value)
{
    setParameter(name, MaterialParameterType::Texture, &value);
}


void Material::setParameterAutoBinding(const std::string& name, AutoBinding autoBinding)
{
    switch (autoBinding)
    {
        case AutoBinding::WorldMatrix:
            setParameter(name, MaterialParameterType::WorldMatrix, nullptr);
            break;
        case AutoBinding::ViewMatrix:
            setParameter(name, MaterialParameterType::ViewMatrix, nullptr);
            break;
        case AutoBinding::ProjectionMatrix:
            setParameter(name, MaterialParameterType::ProjectionMatrix, nullptr);
            break;
        case AutoBinding::WorldViewMatrix:
            setParameter(name, MaterialParameterType::WorldViewMatrix, nullptr);
            break;
        case AutoBinding::ViewProjectionMatrix:
            setParameter(name, MaterialParameterType::ViewProjectionMatrix, nullptr);
            break;
        case AutoBinding::WorldViewProjectionMatrix:
            setParameter(name, MaterialParameterType::WorldViewProjectionMatrix, nullptr);
            break;
        case AutoBinding::InverseTransposedWorldMatrix:
            setParameter(name, MaterialParameterType::InverseTransposedWorldMatrix, nullptr);
            break;
        case AutoBinding::InverseTransposedWorldViewMatrix:
            setParameter(name, MaterialParameterType::InverseTransposedWorldViewMatrix, nullptr);
            break;
        case AutoBinding::CameraWorldPosition:
            setParameter(name, MaterialParameterType::CameraWorldPosition, nullptr);
            break;
        default: break; // TODO debug throw
    }
}


void Material::apply(const RenderContext& context)
{
    applyState();
    
    if (effect)
    {
        effect->apply();
        for (auto& p : parameters)
            p.second->apply(context);
    }
}


void Material::applyState()
{
    renderer->setPolygonFace(polygonFace);
    renderer->setDepthTest(depthTest);
    renderer->setDepthWrite(depthWrite);
    renderer->setDepthFunction(depthFunc);
    renderer->setBlend(transparent);
    renderer->setBlendFactor(srcBlendFactor, dstBlendFactor);
}
