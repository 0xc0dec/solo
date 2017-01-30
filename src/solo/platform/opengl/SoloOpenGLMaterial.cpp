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
#include "SoloOpenGLTexture.h"
#include "SoloCamera.h"
#include "SoloTransform.h"
#include "SoloTexture.h"

using namespace solo;


OpenGLMaterial::OpenGLMaterial(Device *device, sptr<Effect> effect):
    Material(device, effect)
{
    renderer = dynamic_cast<OpenGLRenderer *>(device->getRenderer());
    this->effect = std::dynamic_pointer_cast<OpenGLEffect>(effect);
}


OpenGLMaterial::~OpenGLMaterial()
{
    for (auto &p : uniformHandles)
        renderer->destroyUniform(p.second);
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


void OpenGLMaterial::applyParams(const Camera *camera, Transform *nodeTransform)
{
    // TODO refactor
    // TODO avoid table lookups

    applyScalarParams(floatParams);
    applyScalarParams(vector2Params);
    applyScalarParams(vector3Params);
    applyScalarParams(vector4Params);
    applyScalarParams(matrixParams);

    applyVectorParams(floatArrayParams);
    applyVectorParams(vector2ArrayParams);
    applyVectorParams(vector3ArrayParams);
    applyVectorParams(vector4ArrayParams);
    applyVectorParams(matrixArrayParams);

    for (const auto &p: textureParams)
    {
        renderer->setUniform(uniformHandles[p.first], nullptr, 1);
        p.second->bind();
    }

    for (const auto &p : worldMatrixParams)
    {
        if (nodeTransform)
            renderer->setUniform(uniformHandles[p], nodeTransform->getWorldMatrix().m, 1);
    }

    for (const auto &p : viewMatrixParams)
    {
        if (camera)
            renderer->setUniform(uniformHandles[p], camera->getViewMatrix().m, 1);
    }

    for (const auto &p : projMatrixParams)
    {
        if (camera)
            renderer->setUniform(uniformHandles[p], camera->getProjectionMatrix().m, 1);
    }

    for (const auto &p : worldViewMatrixParams)
    {
        if (nodeTransform && camera)
            renderer->setUniform(uniformHandles[p], nodeTransform->getWorldViewMatrix(camera).m, 1);
    }
        
    for (const auto &p : viewProjMatrixParams)
    {
        if (camera)
            renderer->setUniform(uniformHandles[p], camera->getViewProjectionMatrix().m, 1);
    }
    
    for (const auto &p : worldViewProjMatrixParams)
    {
        if (nodeTransform && camera)
            renderer->setUniform(uniformHandles[p], nodeTransform->getWorldViewProjMatrix(camera).m, 1);
    }
    
    for (const auto &p : invTransWorldMatrixParams)
    {
        if (nodeTransform)
            renderer->setUniform(uniformHandles[p], nodeTransform->getInvTransposedWorldMatrix().m, 1);
    }
    
    for (const auto &p : invTransWorldMatrixParams)
    {
        if (nodeTransform && camera)
            renderer->setUniform(uniformHandles[p], nodeTransform->getInvTransposedWorldViewMatrix(camera).m, 1);
    }

    for (const auto &p : viewProjMatrixParams)
    {
        if (camera)
        {
            auto pos = camera->getTransform()->getWorldPosition();
            renderer->setUniform(uniformHandles[p], &pos, 1);
        }
    }
}


void OpenGLMaterial::setFloatParameter(const std::string &name, float value)
{
    setParam(floatParams, name, UniformType::Float, value);
}


void OpenGLMaterial::setFloatArrayParameter(const std::string &name, const std::vector<float> &value)
{
    setParam(floatArrayParams, name, UniformType::FloatArray, value);
}


void OpenGLMaterial::setVector2Parameter(const std::string& name, const Vector2 &value)
{
    setParam(vector2Params, name, UniformType::Vector2, value);
}


void OpenGLMaterial::setVector2ArrayParameter(const std::string& name, const std::vector<Vector2> &value)
{
    setParam(vector2ArrayParams, name, UniformType::Vector2Array, value);
}


void OpenGLMaterial::setVector3Parameter(const std::string &name, const Vector3 &value)
{
    setParam(vector3Params, name, UniformType::Vector3, value);
}


void OpenGLMaterial::setVector3ArrayParameter(const std::string &name, const std::vector<Vector3> &value)
{
    setParam(vector3ArrayParams, name, UniformType::Vector3Array, value);
}


void OpenGLMaterial::setVector4Parameter(const std::string &name, const Vector4 &value)
{
    setParam(vector4Params, name, UniformType::Vector4, value);
}


void OpenGLMaterial::setVector4ArrayParameter(const std::string &name, const std::vector<Vector4> &value)
{
    setParam(vector4ArrayParams, name, UniformType::Vector4Array, value);
}


void OpenGLMaterial::setMatrixParameter(const std::string &name, const Matrix& value)
{
    setParam(matrixParams, name, UniformType::Matrix, value);
}


void OpenGLMaterial::setMatrixArrayParameter(const std::string &name, const std::vector<Matrix>& value)
{
    setParam(matrixArrayParams, name, UniformType::MatrixArray, value);
}


void OpenGLMaterial::setTextureParameter(const std::string &name, sptr<Texture> value)
{
    if (textureParams.find(name) == textureParams.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Texture, effect->getHandle());
    textureParams[name] = std::dynamic_pointer_cast<OpenGLTexture>(value);
}


void OpenGLMaterial::bindWorldMatrixParameter(const std::string& name)
{
    setAutoBindParam(worldMatrixParams, name, UniformType::Matrix);
}


void OpenGLMaterial::bindViewMatrixParameter(const std::string& name)
{
    setAutoBindParam(viewMatrixParams, name, UniformType::Matrix);
}


void OpenGLMaterial::bindProjectionMatrixParameter(const std::string& name)
{
    setAutoBindParam(projMatrixParams, name, UniformType::Matrix);
}


void OpenGLMaterial::bindWorldViewMatrixParameter(const std::string& name)
{
    setAutoBindParam(worldViewMatrixParams, name, UniformType::Matrix);
}


void OpenGLMaterial::bindViewProjectionMatrixParameter(const std::string& name)
{
    setAutoBindParam(viewProjMatrixParams, name, UniformType::Matrix);
}


void OpenGLMaterial::bindWorldViewProjectionMatrixParameter(const std::string& name)
{
    setAutoBindParam(worldViewProjMatrixParams, name, UniformType::Matrix);
}


void OpenGLMaterial::bindInvTransposedWorldMatrixParameter(const std::string& name)
{
    setAutoBindParam(invTransWorldMatrixParams, name, UniformType::Matrix);
}


void OpenGLMaterial::bindInvTransposedWorldViewMatrixParameter(const std::string& name)
{
    setAutoBindParam(invTransWorldViewMatrixParams, name, UniformType::Matrix);
}


void OpenGLMaterial::bindCameraWorldPositionParameter(const std::string& name)
{
    setAutoBindParam(camWorldPosParams, name, UniformType::Vector3);
}


#endif
