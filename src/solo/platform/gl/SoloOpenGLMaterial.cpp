/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLMaterial.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloOpenGLTexture.h"
#include "SoloOpenGLCamera.h"
#include "SoloTransform.h"
#include "SoloTexture.h"

using namespace solo;


gl::Material::Material(Device *device, sptr<solo::Effect> effect):
    solo::Material(device, effect)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
    this->effect = std::dynamic_pointer_cast<Effect>(effect);
}


void gl::Material::applyState() const
{
    renderer->setFaceCull(faceCull);
    renderer->setPolygonMode(polygonMode);
    renderer->setDepthTest(depthTest);
    renderer->setDepthWrite(depthWrite);
    renderer->setDepthFunction(depthFunc);
    renderer->setBlend(blend);
    renderer->setBlendFactor(srcBlendFactor, dstBlendFactor);
}


void gl::Material::applyParams(const gl::Camera *camera, const Transform *nodeTransform) const
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
        setUniform(p.first, nullptr, 1);
        p.second->bind();
    }

    for (const auto &p : worldMatrixParams)
    {
        if (nodeTransform)
            setUniform(p, nodeTransform->getWorldMatrix().m, 1);
    }

    for (const auto &p : viewMatrixParams)
    {
        if (camera)
            setUniform(p, camera->getViewMatrix().m, 1);
    }

    for (const auto &p : projMatrixParams)
    {
        if (camera)
            setUniform(p, camera->getProjectionMatrix().m, 1);
    }

    for (const auto &p : worldViewMatrixParams)
    {
        if (nodeTransform && camera)
            setUniform(p, nodeTransform->getWorldViewMatrix(camera).m, 1);
    }
        
    for (const auto &p : viewProjMatrixParams)
    {
        if (camera)
            setUniform(p, camera->getViewProjectionMatrix().m, 1);
    }
    
    for (const auto &p : worldViewProjMatrixParams)
    {
        if (nodeTransform && camera)
            setUniform(p, nodeTransform->getWorldViewProjMatrix(camera).m, 1);
    }
    
    for (const auto &p : invTransWorldMatrixParams)
    {
        if (nodeTransform)
            setUniform(p, nodeTransform->getInvTransposedWorldMatrix().m, 1);
    }
    
    for (const auto &p : invTransWorldViewMatrixParams)
    {
        if (nodeTransform && camera)
            setUniform(p, nodeTransform->getInvTransposedWorldViewMatrix(camera).m, 1);
    }

    for (const auto &p : viewProjMatrixParams)
    {
        if (camera)
        {
            auto pos = camera->getTransform()->getWorldPosition();
            setUniform(p, &pos, 1);
        }
    }
}


static bool findUniformInProgram(GLuint program, const char *name, GLint &location, int32_t &index)
{
    GLint activeUniforms;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms);
    if (activeUniforms <= 0)
        return false;

    GLint nameMaxLength;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);
    if (nameMaxLength <= 0)
        return false;

    std::vector<GLchar> rawName(nameMaxLength + 1);
    uint32_t samplerIndex = 0;
    for (GLint i = 0; i < activeUniforms; ++i)
    {
        GLint size;
        GLenum type;

        glGetActiveUniform(program, i, nameMaxLength, nullptr, &size, &type, rawName.data());
        rawName[nameMaxLength] = '\0';
        std::string n = rawName.data();

        // Strip away possible square brackets for array uniforms,
        // they are sometimes present on some platforms
        auto bracketIndex = n.find('[');
        if (bracketIndex != std::string::npos)
            n.erase(bracketIndex);

        uint32_t idx = 0;
        if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE) // TODO other types of samplers
        {
            idx = samplerIndex;
            samplerIndex += size;
        }

        if (n == name)
        {
            location = glGetUniformLocation(program, rawName.data());
            index = idx;
            return true;
        }
    }

    return false;
}


void gl::Material::initUniform(const std::string &name, UniformType type)
{
    GLint location, index;
    auto found = findUniformInProgram(effect->getHandle(), name.c_str(), location, index);
    SL_PANIC_IF(!found, SL_FMT("Could not find uniform '", name, "'"));

    uniformLocations[name] = location;
    uniformIndexes[name] = index;
    uniformTypes[name] = type;
}


void gl::Material::setUniform(const std::string &name, const void *value, uint32_t count) const
{
    // TODO avoid lookup
    auto location = uniformLocations.at(name);
    auto index = uniformIndexes.at(name);
    auto type = uniformTypes.at(name);

    auto floatData = reinterpret_cast<const float *>(value);
    switch (type)
    {
        case UniformType::Float:
            glUniform1f(location, *floatData);
            break;
        case UniformType::FloatArray:
            glUniform1fv(location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Vector2:
            glUniform2f(location, floatData[0], floatData[1]);
            break;
        case UniformType::Vector2Array:
            glUniform2fv(location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Vector3:
            glUniform3f(location, floatData[0], floatData[1], floatData[2]);
            break;
        case UniformType::Vector3Array:
            glUniform3fv(location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Vector4:
            glUniform4f(location, floatData[0], floatData[1], floatData[2], floatData[3]);
            break;
        case UniformType::Vector4Array:
            glUniform4fv(location, static_cast<GLsizei>(count), floatData);
            break;
        case UniformType::Matrix:
            glUniformMatrix4fv(location, 1, GL_FALSE, floatData);
            break;
        case UniformType::MatrixArray:
            glUniformMatrix4fv(location, static_cast<GLsizei>(count), GL_FALSE, floatData);
            break;
        case UniformType::Texture:
            glActiveTexture(GL_TEXTURE0 + index);
            glUniform1i(location, index);
            break;
        case UniformType::TextureArray:
            break; // TODO
        default:
            break;
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
        initUniform(name, UniformType::Texture);
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
