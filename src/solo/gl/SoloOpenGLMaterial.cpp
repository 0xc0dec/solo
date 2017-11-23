/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLMaterial.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloCamera.h"
#include "SoloOpenGLTexture.h"
#include "SoloTransform.h"
#include "SoloTexture.h"

using namespace solo;

gl::OpenGLMaterial::OpenGLMaterial(sptr<Effect> effect):
    effect(std::dynamic_pointer_cast<OpenGLEffect>(effect))
{
}

void gl::OpenGLMaterial::applyParams(const Camera *camera, const Transform *nodeTransform) const
{
    for (const auto &p : appliers)
        p.second(camera, nodeTransform);
}

void gl::OpenGLMaterial::setFloatParameter(const str &name, float value)
{
    setParameter(name, [value](GLuint location, GLuint index)
    {
        return [location, index, value](const Camera *, const Transform *)
        {
            glUniform1f(location, value);
        };
    });
}

void gl::OpenGLMaterial::setVector2Parameter(const str &name, const Vector2 &value)
{
    setParameter(name, [value](GLuint location, GLuint index)
    {
        return [location, index, value](const Camera *, const Transform *)
        {
            glUniform2f(location, value.x, value.y);
        };
    });
}

void gl::OpenGLMaterial::setVector3Parameter(const str &name, const Vector3 &value)
{
    setParameter(name, [value](GLuint location, GLuint index)
    {
        return [location, index, value](const Camera *, const Transform *)
        {
            glUniform3f(location, value.x, value.y, value.z);
        };
    });
}

void gl::OpenGLMaterial::setVector4Parameter(const str &name, const Vector4 &value)
{
    setParameter(name, [value](GLuint location, GLuint index)
    {
        return [location, index, value](const Camera *, const Transform *)
        {
            glUniform4f(location, value.x, value.y, value.z, value.w);
        };
    });
}

void gl::OpenGLMaterial::setMatrixParameter(const str &name, const Matrix &value)
{
    setParameter(name, [value](GLuint location, GLuint index)
    {
        return [location, index, value](const Camera *, const Transform *)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, value.m);
        };
    });
}

void gl::OpenGLMaterial::setTextureParameter(const str &name, sptr<solo::Texture> value)
{
    auto tex = std::dynamic_pointer_cast<OpenGLTexture>(value);
    setParameter(name, [tex](GLuint location, GLuint index)
    {
        return [location, index, tex](const Camera *, const Transform *)
        {
            glActiveTexture(GL_TEXTURE0 + index);
            glUniform1i(location, index);
            tex->bind();
        };
    });
}

void gl::OpenGLMaterial::bindParameter(const str &name, BindParameterSemantics semantics)
{
    switch (semantics)
    {
        case BindParameterSemantics::WorldMatrix:
        {
            setParameter(name, [](GLuint location, GLuint index)
            {
                return [location, index](const Camera *camera, const Transform *nodeTransform)
                {
                    if (nodeTransform)
                    {
                        auto data = nodeTransform->getWorldMatrix().m;
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case BindParameterSemantics::ViewMatrix:
        {
            setParameter(name, [](GLuint location, GLuint index)
            {
                return [location, index](const Camera *camera, const Transform *nodeTransform)
                {
                    if (camera)
                    {
                        auto data = camera->getViewMatrix().m;
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case BindParameterSemantics::ProjectionMatrix:
        {
            setParameter(name, [](GLuint location, GLuint index)
            {
                return [location, index](const Camera *camera, const Transform *nodeTransform)
                {
                    if (camera)
                    {
                        auto data = camera->getProjectionMatrix().m;
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case BindParameterSemantics::WorldViewMatrix:
        {
            setParameter(name, [](GLuint location, GLuint index)
            {
                return [location, index](const Camera *camera, const Transform *nodeTransform)
                {
                    if (nodeTransform && camera)
                    {
                        auto data = nodeTransform->getWorldViewMatrix(camera).m;
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case BindParameterSemantics::ViewProjectionMatrix:
        {
            setParameter(name, [](GLuint location, GLuint index)
            {
                return [location, index](const Camera *camera, const Transform *nodeTransform)
                {
                    if (camera)
                    {
                        auto data = camera->getViewProjectionMatrix().m;
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case BindParameterSemantics::WorldViewProjectionMatrix:
        {
            setParameter(name, [](GLuint location, GLuint index)
            {
                return [location, index](const Camera *camera, const Transform *nodeTransform)
                {
                    if (nodeTransform && camera)
                    {
                        auto data = nodeTransform->getWorldViewProjMatrix(camera).m;
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case BindParameterSemantics::InverseTransposedWorldMatrix:
        {
            setParameter(name, [](GLuint location, GLuint index)
            {
                return [location, index](const Camera *camera, const Transform *nodeTransform)
                {
                    if (nodeTransform)
                    {
                        auto data = nodeTransform->getInvTransposedWorldMatrix().m;
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case BindParameterSemantics::InverseTransposedWorldViewMatrix:
        {
            setParameter(name, [](GLuint location, GLuint index)
            {
                return [location, index](const Camera *camera, const Transform *nodeTransform)
                {
                    if (nodeTransform && camera)
                    {
                        auto data = nodeTransform->getInvTransposedWorldViewMatrix(camera).m;
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case BindParameterSemantics::CameraWorldPosition:
        {
            setParameter(name, [](GLuint location, GLuint index)
            {
                return [location, index](const Camera *camera, const Transform *nodeTransform)
                {
                    if (camera)
                    {
                        auto pos = camera->getTransform()->getWorldPosition();
                        glUniform3f(location, pos.x, pos.y, pos.z);
                    }
                };
            });
            break;
        }

        default:
            SL_PANIC("Unsupported bind parameter semantics");
    }
}

void gl::OpenGLMaterial::setParameter(const str &paramName, std::function<ParameterApplier(GLuint, GLint)> getApplier)
{
    const auto info = effect->getUniformInfo(paramName);
    appliers[paramName] = getApplier(info.location, info.samplerIndex);
}

#endif
