/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLMaterial.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloOpenGLTexture.h"
#include "SoloCamera.h"
#include "SoloTransform.h"
#include "SoloTexture.h"

using namespace solo;


gl::Material::Material(Device *device, sptr<solo::Effect> effect):
    solo::Material(device, effect)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
    this->effect = std::dynamic_pointer_cast<Effect>(effect);
}


gl::Material::~Material()
{
    for (auto &p : uniformHandles)
        renderer->destroyUniform(p.second);
}


void gl::Material::applyImpl(const Camera *camera, Transform *nodeTransform)
{
    applyState();
    applyParams(camera, nodeTransform);
}


void gl::Material::applyState()
{
    renderer->setFaceCull(faceCull);
    renderer->setPolygonMode(polygonMode);
    renderer->setDepthTest(depthTest);
    renderer->setDepthWrite(depthWrite);
    renderer->setDepthFunction(depthFunc);
    renderer->setBlend(blend);
    renderer->setBlendFactor(srcBlendFactor, dstBlendFactor);
}


void gl::Material::applyParams(const Camera *camera, Transform *nodeTransform)
{
    if (!this->effect) // TODO needed?
        return;

    // TODO refactor
    // TODO avoid table lookups

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


void gl::Material::setFloatParameter(const std::string &name, float value)
{
    setParam(floatParams, name, UniformType::Float, value);
}


void gl::Material::setFloatArrayParameter(const std::string &name, const std::vector<float> &value)
{
    setParam(floatArrayParams, name, UniformType::FloatArray, value);
}


void gl::Material::setVector2Parameter(const std::string& name, const Vector2 &value)
{
    setParam(vector2Params, name, UniformType::Vector2, value);
}


void gl::Material::setVector2ArrayParameter(const std::string& name, const std::vector<Vector2> &value)
{
    setParam(vector2ArrayParams, name, UniformType::Vector2Array, value);
}


void gl::Material::setVector3Parameter(const std::string &name, const Vector3 &value)
{
    setParam(vector3Params, name, UniformType::Vector3, value);
}


void gl::Material::setVector3ArrayParameter(const std::string &name, const std::vector<Vector3> &value)
{
    setParam(vector3ArrayParams, name, UniformType::Vector3Array, value);
}


void gl::Material::setVector4Parameter(const std::string &name, const Vector4 &value)
{
    setParam(vector4Params, name, UniformType::Vector4, value);
}


void gl::Material::setVector4ArrayParameter(const std::string &name, const std::vector<Vector4> &value)
{
    setParam(vector4ArrayParams, name, UniformType::Vector4Array, value);
}


void gl::Material::setMatrixParameter(const std::string &name, const Matrix& value)
{
    setParam(matrixParams, name, UniformType::Matrix, value);
}


void gl::Material::setMatrixArrayParameter(const std::string &name, const std::vector<Matrix>& value)
{
    setParam(matrixArrayParams, name, UniformType::MatrixArray, value);
}


void gl::Material::setTextureParameter(const std::string &name, sptr<solo::Texture> value)
{
    if (textureParams.find(name) == textureParams.end())
        uniformHandles[name] = renderer->createUniform(name.c_str(), UniformType::Texture, effect->getHandle());
    textureParams[name] = std::dynamic_pointer_cast<Texture>(value);
}


void gl::Material::bindWorldMatrixParameter(const std::string& name)
{
    setAutoBindParam(worldMatrixParams, name, UniformType::Matrix);
}


void gl::Material::bindViewMatrixParameter(const std::string& name)
{
    setAutoBindParam(viewMatrixParams, name, UniformType::Matrix);
}


void gl::Material::bindProjectionMatrixParameter(const std::string& name)
{
    setAutoBindParam(projMatrixParams, name, UniformType::Matrix);
}


void gl::Material::bindWorldViewMatrixParameter(const std::string& name)
{
    setAutoBindParam(worldViewMatrixParams, name, UniformType::Matrix);
}


void gl::Material::bindViewProjectionMatrixParameter(const std::string& name)
{
    setAutoBindParam(viewProjMatrixParams, name, UniformType::Matrix);
}


void gl::Material::bindWorldViewProjectionMatrixParameter(const std::string& name)
{
    setAutoBindParam(worldViewProjMatrixParams, name, UniformType::Matrix);
}


void gl::Material::bindInvTransposedWorldMatrixParameter(const std::string& name)
{
    setAutoBindParam(invTransWorldMatrixParams, name, UniformType::Matrix);
}


void gl::Material::bindInvTransposedWorldViewMatrixParameter(const std::string& name)
{
    setAutoBindParam(invTransWorldViewMatrixParams, name, UniformType::Matrix);
}


void gl::Material::bindCameraWorldPositionParameter(const std::string& name)
{
    setAutoBindParam(camWorldPosParams, name, UniformType::Vector3);
}


#endif
