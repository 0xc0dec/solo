/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloOpenGLMaterial.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloDevice.h"
#include "SoloCamera.h"
#include "SoloOpenGLTexture.h"
#include "SoloTransform.h"
#include "SoloTexture.h"

using namespace solo;

OpenGLMaterial::OpenGLMaterial(sptr<Effect> effect):
    effect_(std::static_pointer_cast<OpenGLEffect>(effect))
{
}

void OpenGLMaterial::applyParams(const Camera *camera, const Transform *nodeTransform) const
{
    for (const auto &p : appliers_)
        p.second(camera, nodeTransform);
}

void OpenGLMaterial::setFloatParameter(const str &name, float value)
{
    setParameter(name, [value](GLuint location, GLuint)
    {
        return [location, value](const Camera *, const Transform *)
        {
            glUniform1f(location, value);
        };
    });
}

void OpenGLMaterial::setVector2Parameter(const str &name, const Vector2 &value)
{
    setParameter(name, [value](GLuint location, GLuint)
    {
        return [location, value](const Camera *, const Transform *)
        {
            glUniform2f(location, value.x(), value.y());
        };
    });
}

void OpenGLMaterial::setVector3Parameter(const str &name, const Vector3 &value)
{
    setParameter(name, [value](GLuint location, GLuint)
    {
        return [location, value](const Camera *, const Transform *)
        {
            glUniform3f(location, value.x(), value.y(), value.z());
        };
    });
}

void OpenGLMaterial::setVector4Parameter(const str &name, const Vector4 &value)
{
    setParameter(name, [value](GLuint location, GLuint)
    {
        return [location, value](const Camera *, const Transform *)
        {
            glUniform4f(location, value.x(), value.y(), value.z(), value.w());
        };
    });
}

void OpenGLMaterial::setMatrixParameter(const str &name, const Matrix &value)
{
    setParameter(name, [value](GLuint location, GLuint)
    {
        return [location, value](const Camera *, const Transform *)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, value.columns());
        };
    });
}

void OpenGLMaterial::setTextureParameter(const str &name, sptr<Texture> value)
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

void OpenGLMaterial::bindParameter(const str &name, ParameterBinding binding)
{
    switch (binding)
    {
        case ParameterBinding::WorldMatrix:
        {
            setParameter(name, [](GLuint location, GLuint)
            {
                return [location](const Camera *, const Transform *nodeTransform)
                {
                    if (nodeTransform)
                    {
                        auto data = nodeTransform->worldMatrix().columns();
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case ParameterBinding::ViewMatrix:
        {
            setParameter(name, [](GLuint location, GLuint)
            {
                return [location](const Camera *camera, const Transform *)
                {
                    if (camera)
                    {
                        auto data = camera->viewMatrix().columns();
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case ParameterBinding::ProjectionMatrix:
        {
            setParameter(name, [](GLuint location, GLuint)
            {
                return [location](const Camera *camera, const Transform *)
                {
                    if (camera)
                    {
                        auto data = camera->projectionMatrix().columns();
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case ParameterBinding::WorldViewMatrix:
        {
            setParameter(name, [](GLuint location, GLuint)
            {
                return [location](const Camera *camera, const Transform *nodeTransform)
                {
                    if (nodeTransform && camera)
                    {
                        auto data = nodeTransform->worldViewMatrix(camera).columns();
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case ParameterBinding::ViewProjectionMatrix:
        {
            setParameter(name, [](GLuint location, GLuint)
            {
                return [location](const Camera *camera, const Transform *)
                {
                    if (camera)
                    {
                        auto data = camera->viewProjectionMatrix().columns();
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case ParameterBinding::WorldViewProjectionMatrix:
        {
            setParameter(name, [](GLuint location, GLuint)
            {
                return [location](const Camera *camera, const Transform *nodeTransform)
                {
                    if (nodeTransform && camera)
                    {
                        auto data = nodeTransform->worldViewProjMatrix(camera).columns();
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case ParameterBinding::InverseTransposedWorldMatrix:
        {
            setParameter(name, [](GLuint location, GLuint)
            {
                return [location](const Camera *, const Transform *nodeTransform)
                {
                    if (nodeTransform)
                    {
                        auto data = nodeTransform->invTransposedWorldMatrix().columns();
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case ParameterBinding::InverseTransposedWorldViewMatrix:
        {
            setParameter(name, [](GLuint location, GLuint)
            {
                return [location](const Camera *camera, const Transform *nodeTransform)
                {
                    if (nodeTransform && camera)
                    {
                        auto data = nodeTransform->invTransposedWorldViewMatrix(camera).columns();
                        glUniformMatrix4fv(location, 1, GL_FALSE, data);
                    }
                };
            });
            break;
        }

        case ParameterBinding::CameraWorldPosition:
        {
            setParameter(name, [](GLuint location, GLuint)
            {
                return [location](const Camera *camera, const Transform *)
                {
                    if (camera)
                    {
                        auto pos = camera->transform()->worldPosition();
                        glUniform3f(location, pos.x(), pos.y(), pos.z());
                    }
                };
            });
            break;
        }

    	default:
            asrt(false, "Unsupported parameter binding");
    }
}

void OpenGLMaterial::setParameter(const str &paramName, const std::function<ParameterApplier(GLuint, GLint)> &getApplier)
{
    auto name = paramName;
    const auto idx = paramName.find_last_of(':');
    if (idx != std::string::npos)
        name.replace(idx, 1, "_");
    const auto info = effect_->uniformInfo(name);
    appliers_[paramName] = getApplier(info.location, info.samplerIndex);
}

#endif
