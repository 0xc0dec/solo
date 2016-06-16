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


void Material::bindWorldMatrixParameter(const std::string& name)
{
    setParameter(name, MaterialParameterType::WorldMatrix, nullptr);
}


void Material::bindViewMatrixParameter(const std::string& name)
{
    setParameter(name, MaterialParameterType::ViewMatrix, nullptr);
}


void Material::bindProjectionMatrixParameter(const std::string& name)
{
    setParameter(name, MaterialParameterType::ProjectionMatrix, nullptr);
}


void Material::bindWorldViewMatrixParameter(const std::string& name)
{
    setParameter(name, MaterialParameterType::WorldViewMatrix, nullptr);
}


void Material::bindViewProjectionMatrixParameter(const std::string& name)
{
    setParameter(name, MaterialParameterType::ViewProjectionMatrix, nullptr);
}


void Material::bindWorldViewProjectionMatrixParameter(const std::string& name)
{
    setParameter(name, MaterialParameterType::WorldViewProjectionMatrix, nullptr);
}


void Material::bindInvTransposedWorldMatrixParameter(const std::string& name)
{
    setParameter(name, MaterialParameterType::InverseTransposedWorldMatrix, nullptr);
}


void Material::bindInvTransposedWorldViewMatrixParameter(const std::string& name)
{
    setParameter(name, MaterialParameterType::InverseTransposedWorldViewMatrix, nullptr);
}


void Material::bindCameraWorldPositionParameter(const std::string& name)
{
    setParameter(name, MaterialParameterType::CameraWorldPosition, nullptr);
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
    renderer->setFaceCull(faceCull);
    renderer->setPolygonMode(polygonMode);
    renderer->setDepthTest(depthTest);
    renderer->setDepthWrite(depthWrite);
    renderer->setDepthFunction(depthFunc);
    renderer->setBlend(transparent);
    renderer->setBlendFactor(srcBlendFactor, dstBlendFactor);
}
