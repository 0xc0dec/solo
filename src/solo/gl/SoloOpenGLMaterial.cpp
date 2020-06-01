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

OpenGLMaterial::OpenGLMaterial(const sptr<Effect> &effect):
    effect_(std::static_pointer_cast<OpenGLEffect>(effect))
{
}

void OpenGLMaterial::applyParams(const Camera *camera, const Transform *nodeTransform) const
{
    for (const auto &d: descriptors_)
        d.second.write(d.second.location, d.second.samplerIndex, camera, nodeTransform);
}

void OpenGLMaterial::setFloatParameter(const str &name, float value)
{
	setParameter(name, [value](GLuint location, GLuint, const Camera *, const Transform *)
	{
		glUniform1f(location, value);
	});
}

void OpenGLMaterial::setVector2Parameter(const str &name, const Vector2 &value)
{
    setParameter(name, [value](GLuint location, GLuint, const Camera *, const Transform *)
    {
        glUniform2f(location, value.x(), value.y());
    });
}

void OpenGLMaterial::setVector3Parameter(const str &name, const Vector3 &value)
{
    setParameter(name, [value](GLuint location, GLuint, const Camera *, const Transform *)
	{
    	glUniform3f(location, value.x(), value.y(), value.z());
    });
}

void OpenGLMaterial::setVector4Parameter(const str &name, const Vector4 &value)
{
    setParameter(name, [value](GLuint location, GLuint, const Camera *, const Transform *)
    {
        glUniform4f(location, value.x(), value.y(), value.z(), value.w());
    });
}

void OpenGLMaterial::setMatrixParameter(const str &name, const Matrix &value)
{
    setParameter(name, [value](GLuint location, GLuint, const Camera *, const Transform *)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, value.columns());
    });
}

void OpenGLMaterial::setTextureParameter(const str &name, sptr<Texture> value)
{
    auto tex = std::dynamic_pointer_cast<OpenGLTexture>(value);
    setParameter(name, [tex](GLuint location, GLuint index, const Camera *, const Transform *)
    {
        glActiveTexture(GL_TEXTURE0 + index);
        glUniform1i(location, index);
        tex->bind();
    });
}

void OpenGLMaterial::bindFloatParameter(const str &name, const std::function<float()> &valueGetter)
{
	setParameter(name, [valueGetter](GLuint location, GLuint, const Camera *, const Transform *)
    {
        glUniform1f(location, valueGetter());
    });
}

void OpenGLMaterial::bindVector2Parameter(const str &name, const std::function<Vector2()> &valueGetter)
{
	setParameter(name, [valueGetter](GLuint location, GLuint, const Camera *, const Transform *)
    {
        const auto val = valueGetter();
        glUniform2f(location, val.x(), val.y());
    });
}

void OpenGLMaterial::bindVector3Parameter(const str &name, const std::function<Vector3()> &valueGetter)
{
	setParameter(name, [valueGetter](GLuint location, GLuint, const Camera *, const Transform *)
    {
        const auto val = valueGetter();
        glUniform3f(location, val.x(), val.y(), val.z());
    });
}

void OpenGLMaterial::bindVector4Parameter(const str &name, const std::function<Vector4()> &valueGetter)
{
	setParameter(name, [valueGetter](GLuint location, GLuint, const Camera *, const Transform *)
    {
        const auto val = valueGetter();
        glUniform4f(location, val.x(), val.y(), val.z(), val.w());
    });
}

void OpenGLMaterial::bindMatrixParameter(const str &name, const std::function<Matrix()> &valueGetter)
{
	setParameter(name, [valueGetter](GLuint location, GLuint, const Camera *, const Transform *)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, valueGetter().columns());
    });
}

void OpenGLMaterial::bindParameter(const str &name, ParameterBinding binding)
{
    switch (binding)
    {
        case ParameterBinding::WorldMatrix:
        {
            setParameter(name, [](GLuint location, GLuint, const Camera *, const Transform *nodeTransform)
            {
                if (nodeTransform)
                {
                    const auto data = nodeTransform->worldMatrix().columns();
                    glUniformMatrix4fv(location, 1, GL_FALSE, data);
                }
            });
            break;
        }

        case ParameterBinding::ViewMatrix:
        {
            setParameter(name, [](GLuint location, GLuint, const Camera *camera, const Transform *)
            {
                if (camera)
                {
                    const auto data = camera->viewMatrix().columns();
                    glUniformMatrix4fv(location, 1, GL_FALSE, data);
                }
            });
            break;
        }

        case ParameterBinding::ProjectionMatrix:
        {
            setParameter(name, [](GLuint location, GLuint, const Camera *camera, const Transform *)
            {
                if (camera)
                {
                    const auto data = camera->projectionMatrix().columns();
                    glUniformMatrix4fv(location, 1, GL_FALSE, data);
                }
            });
            break;
        }

        case ParameterBinding::WorldViewMatrix:
        {
            setParameter(name, [](GLuint location, GLuint, const Camera *camera, const Transform *nodeTransform)
            {
                if (nodeTransform && camera)
                {
                    const auto data = nodeTransform->worldViewMatrix(camera).columns();
                    glUniformMatrix4fv(location, 1, GL_FALSE, data);
                }
            });
            break;
        }

        case ParameterBinding::ViewProjectionMatrix:
        {
            setParameter(name, [](GLuint location, GLuint, const Camera *camera, const Transform *)
            {
                if (camera)
                {
                    const auto data = camera->viewProjectionMatrix().columns();
                    glUniformMatrix4fv(location, 1, GL_FALSE, data);
                }
            });
            break;
        }

        case ParameterBinding::WorldViewProjectionMatrix:
        {
            setParameter(name, [](GLuint location, GLuint, const Camera *camera, const Transform *nodeTransform)
            {
                if (nodeTransform && camera)
                {
                    const auto data = nodeTransform->worldViewProjMatrix(camera).columns();
                    glUniformMatrix4fv(location, 1, GL_FALSE, data);
                }
            });
            break;
        }

        case ParameterBinding::InverseTransposedWorldMatrix:
        {
            setParameter(name, [](GLuint location, GLuint, const Camera *, const Transform *nodeTransform)
            {
                if (nodeTransform)
                {
                    const auto data = nodeTransform->invTransposedWorldMatrix().columns();
                    glUniformMatrix4fv(location, 1, GL_FALSE, data);
                }
            });
            break;
        }

        case ParameterBinding::InverseTransposedWorldViewMatrix:
        {
            setParameter(name, [](GLuint location, GLuint, const Camera *camera, const Transform *nodeTransform)
            {
                if (nodeTransform && camera)
                {
                    const auto data = nodeTransform->invTransposedWorldViewMatrix(camera).columns();
                    glUniformMatrix4fv(location, 1, GL_FALSE, data);
                }
            });
            break;
        }

        case ParameterBinding::CameraWorldPosition:
        {
            setParameter(name, [](GLuint location, GLuint, const Camera *camera, const Transform *)
            {
                if (camera)
                {
                    auto pos = camera->transform()->worldPosition();
                    glUniform3f(location, pos.x(), pos.y(), pos.z());
                }
            });
            break;
        }

    	default:
    		panic("Unsupported parameter binding");
    }
}

void OpenGLMaterial::setParameter(const str &paramName, const ParameterWriter &writer)
{
	auto name = paramName;
    const auto idx = paramName.find_last_of(':');
    if (idx != std::string::npos)
        name.replace(idx, 1, "_");
	panicIf(!effect_->hasUniform(name), "Unknown material parameter ", paramName);
    const auto info = effect_->uniformInfo(name);
	descriptors_[paramName] = { info.location, info.samplerIndex, writer };
}

#endif
