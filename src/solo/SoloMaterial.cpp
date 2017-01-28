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

#include "SoloMaterial.h"
#include "SoloTexture.h"
#include "SoloDevice.h"
#include "SoloMaterialParameter.h"
#include "platform/opengl/SoloOpenGLMaterial.h"
#include "platform/null/SoloNullMaterial.h"

using namespace solo;


auto Material::create(Device *device, sptr<Effect> effect) -> sptr<Material>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLMaterial>(device, effect);
#endif
        default:
            return std::make_shared<NullMaterial>(device, effect);
    }
}


Material::Material(Device *device, sptr<Effect> effect):
    device(device),
    effect(effect)
{
}


void Material::setParameter(const std::string &name, MaterialParameterType type, const void *value)
{
    auto where = parameters.find(name);
    if (where == parameters.end() || where->second->getType() != type)
        parameters[name] = MaterialParameter::create(device, effect.get(), type, name.c_str());
    parameters.at(name)->setValue(value);
}


void Material::setFloatParameter(const std::string &name, float value)
{
    setParameter(name, MaterialParameterType::Float, &value);
}


void Material::setFloatArrayParameter(const std::string &name, const std::vector<float> &value)
{
    setParameter(name, MaterialParameterType::FloatArray, &value);
}


void Material::setVector2Parameter(const std::string &name, const Vector2 &value)
{
    setParameter(name, MaterialParameterType::Vector2, &value);
}


void Material::setVector2ArrayParameter(const std::string &name, const std::vector<Vector2> &value)
{
    setParameter(name, MaterialParameterType::Vector2Array, &value);
}


void Material::setVector3Parameter(const std::string &name, const Vector3 &value)
{
    setParameter(name, MaterialParameterType::Vector3, &value);
}


void Material::setVector3ArrayParameter(const std::string &name, const std::vector<Vector3> &value)
{
    setParameter(name, MaterialParameterType::Vector3Array, &value);
}


void Material::setVector4Parameter(const std::string &name, const Vector4 &value)
{
    setParameter(name, MaterialParameterType::Vector4, &value);
}


void Material::setVector4ArrayParameter(const std::string &name, const std::vector<Vector4> &value)
{
    setParameter(name, MaterialParameterType::Vector4Array, &value);
}


void Material::setMatrixParameter(const std::string &name, const Matrix &value)
{
    setParameter(name, MaterialParameterType::Matrix, &value);
}


void Material::setMatrixArrayParameter(const std::string &name, const std::vector<Matrix> &value)
{
    setParameter(name, MaterialParameterType::MatrixArray, &value);
}


void Material::setTextureParameter(const std::string &name, sptr<Texture> value)
{
    setParameter(name, MaterialParameterType::Texture, &value);
}


void Material::bindWorldMatrixParameter(const std::string &name)
{
    setParameter(name, MaterialParameterType::WorldMatrix, nullptr);
}


void Material::bindViewMatrixParameter(const std::string &name)
{
    setParameter(name, MaterialParameterType::ViewMatrix, nullptr);
}


void Material::bindProjectionMatrixParameter(const std::string &name)
{
    setParameter(name, MaterialParameterType::ProjectionMatrix, nullptr);
}


void Material::bindWorldViewMatrixParameter(const std::string &name)
{
    setParameter(name, MaterialParameterType::WorldViewMatrix, nullptr);
}


void Material::bindViewProjectionMatrixParameter(const std::string &name)
{
    setParameter(name, MaterialParameterType::ViewProjectionMatrix, nullptr);
}


void Material::bindWorldViewProjectionMatrixParameter(const std::string &name)
{
    setParameter(name, MaterialParameterType::WorldViewProjectionMatrix, nullptr);
}


void Material::bindInvTransposedWorldMatrixParameter(const std::string &name)
{
    setParameter(name, MaterialParameterType::InverseTransposedWorldMatrix, nullptr);
}


void Material::bindInvTransposedWorldViewMatrixParameter(const std::string &name)
{
    setParameter(name, MaterialParameterType::InverseTransposedWorldViewMatrix, nullptr);
}


void Material::bindCameraWorldPositionParameter(const std::string &name)
{
    setParameter(name, MaterialParameterType::CameraWorldPosition, nullptr);
}


void Material::apply(const Camera *camera, Transform *nodeTransform)
{
    applyState();

    if (effect)
    {
        effect->apply();
        for (auto &p : parameters)
            p.second->apply(camera, nodeTransform);
    }
}
