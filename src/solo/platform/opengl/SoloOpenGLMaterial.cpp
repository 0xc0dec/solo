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

#include "SoloOpenGLMaterial.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloOpenGLRenderer.h"
#include "SoloOpenGLEffect.h"

using namespace solo;


OpenGLMaterial::OpenGLMaterial(Device *device, sptr<Effect> effect):
    Material(device, effect)
{
    renderer = dynamic_cast<OpenGLRenderer *>(device->getRenderer());
    this->effect = std::dynamic_pointer_cast<OpenGLEffect>(effect);
}


void OpenGLMaterial::applyState()
{
    renderer->setFaceCull(faceCull);
    renderer->setPolygonMode(polygonMode);
    renderer->setDepthTest(depthTest);
    renderer->setDepthWrite(depthWrite);
    renderer->setDepthFunction(depthFunc);
    renderer->setBlend(blend);
    renderer->setBlendFactor(srcBlendFactor, dstBlendFactor);
}


void OpenGLMaterial::applyParams()
{
    for (const auto &p: floatParams)
        renderer->setUniform(uniformHandles[p.first], &p.second, 1); // TODO avoid handle lookup
    // TODO etc...
}


void OpenGLMaterial::setFloatParameter2(const std::string &name, float value)
{
    if (floatParams.find(name) == floatParams.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Float, effect->getHandle());
    floatParams[name] = value;
}


void OpenGLMaterial::setFloatArrayParameter2(const std::string &name, const std::vector<float> &value)
{
    if (floatParams.find(name) == floatParams.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::FloatArray, effect->getHandle());
    floatArrayParams[name] = value;
}


void OpenGLMaterial::setVector2Parameter2(const std::string& name, const Vector2 &value)
{
    if (vector2Params.find(name) == vector2Params.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Vector2, effect->getHandle());
    vector2Params[name] = value;
}


void OpenGLMaterial::setVector2ArrayParameter2(const std::string& name, const std::vector<Vector2> &value)
{
    if (vector2ArrayParams.find(name) == vector2ArrayParams.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Vector2Array, effect->getHandle());
    vector2ArrayParams[name] = value;
}


void OpenGLMaterial::setVector3Parameter2(const std::string &name, const Vector3 &value)
{
    if (vector3Params.find(name) == vector3Params.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Vector3, effect->getHandle());
    vector3Params[name] = value;
}


void OpenGLMaterial::setVector3ArrayParameter2(const std::string &name, const std::vector<Vector3> &value)
{
    if (vector3ArrayParams.find(name) == vector3ArrayParams.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Vector3Array, effect->getHandle());
    vector3ArrayParams[name] = value;
}


void OpenGLMaterial::setVector4Parameter2(const std::string &name, const Vector4 &value)
{
    if (vector4Params.find(name) == vector4Params.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Vector4, effect->getHandle());
    vector4Params[name] = value;
}


void OpenGLMaterial::setVector4ArrayParameter2(const std::string &name, const std::vector<Vector4> &value)
{
    if (vector4ArrayParams.find(name) == vector4ArrayParams.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Vector4Array, effect->getHandle());
    vector4ArrayParams[name] = value;
}


void OpenGLMaterial::setMatrixParameter2(const std::string &name, const Matrix& value)
{
    if (matrixParams.find(name) == matrixParams.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Matrix, effect->getHandle());
    matrixParams[name] = value;
}


void OpenGLMaterial::setMatrixArrayParameter2(const std::string &name, const std::vector<Matrix>& value)
{
    if (matrixArrayParams.find(name) == matrixArrayParams.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::MatrixArray, effect->getHandle());
    matrixArrayParams[name] = value;
}


void OpenGLMaterial::setTextureParameter2(const std::string &name, sptr<Texture> value)
{
    if (textureParams.find(name) == textureParams.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Texture, effect->getHandle());
    textureParams[name] = value;
}


void OpenGLMaterial::bindWorldMatrixParameter2(const std::string& name)
{
    if (uniformHandles.find(name) == uniformHandles.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Matrix, effect->getHandle());
    worldMatrixParams.insert(name);
}


void OpenGLMaterial::bindViewMatrixParameter2(const std::string& name)
{
    if (uniformHandles.find(name) == uniformHandles.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Matrix, effect->getHandle());
    viewMatrixParams.insert(name);
}


void OpenGLMaterial::bindProjectionMatrixParameter2(const std::string& name)
{
    if (uniformHandles.find(name) == uniformHandles.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Matrix, effect->getHandle());
    projMatrixParams.insert(name);
}


void OpenGLMaterial::bindWorldViewMatrixParameter2(const std::string& name)
{
    if (uniformHandles.find(name) == uniformHandles.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Matrix, effect->getHandle());
    worldViewMatrixParams.insert(name);
}


void OpenGLMaterial::bindViewProjectionMatrixParameter2(const std::string& name)
{
    if (uniformHandles.find(name) == uniformHandles.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Matrix, effect->getHandle());
    viewProjMatrixParams.insert(name);
}


void OpenGLMaterial::bindWorldViewProjectionMatrixParameter2(const std::string& name)
{
    if (uniformHandles.find(name) == uniformHandles.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Matrix, effect->getHandle());
    worldViewProjMatrixParams.insert(name);
}


void OpenGLMaterial::bindInvTransposedWorldMatrixParameter2(const std::string& name)
{
    if (uniformHandles.find(name) == uniformHandles.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Matrix, effect->getHandle());
    invTransWorldMatrixParams.insert(name);
}


void OpenGLMaterial::bindInvTransposedWorldViewMatrixParameter2(const std::string& name)
{
    if (uniformHandles.find(name) == uniformHandles.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Matrix, effect->getHandle());
    invTransWorldViewMatrixParams.insert(name);
}


void OpenGLMaterial::bindCameraWorldPositionParameter2(const std::string& name)
{
    if (uniformHandles.find(name) == uniformHandles.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Vector3, effect->getHandle());
    camWorldPosParams.insert(name);
}


#endif
