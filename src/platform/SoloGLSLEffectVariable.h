#pragma once

#include <GL/glew.h>
#include "SoloBase.h"
#include "SoloEffectVariable.h"

namespace solo
{
	class GLSLEffectVariable : public EffectVariable
	{
	public:
		GLSLEffectVariable(const std::string& name, GLint location, GLenum type, unsigned index);

		virtual void setValue(float value) override;
		virtual void setValue(const float* values, unsigned count) override;
		virtual void setValue(int value) override;
		virtual void setValue(const int* values, unsigned count) override;
		virtual void setValue(const Matrix& value) override;
		virtual void setValue(const Matrix* values, unsigned count) override;
		virtual void setValue(const Vector2& value) override;
		virtual void setValue(const Vector2* values, unsigned count) override;
		virtual void setValue(const Vector3& value) override;
		virtual void setValue(const Vector3* values, unsigned count) override;
		virtual void setValue(const Vector4& value) override;
		virtual void setValue(const Vector4* values, unsigned count) override;
		virtual void setValue(const shared<Texture> sampler) override;
		virtual void setValue(const std::vector<shared<Texture>>& samplers, unsigned count) override;

	private:
		GLSLEffectVariable(const GLSLEffectVariable& other) = delete;
		GLSLEffectVariable(GLSLEffectVariable&& other) = delete;
		GLSLEffectVariable& operator=(const GLSLEffectVariable& other) = delete;
		GLSLEffectVariable& operator=(GLSLEffectVariable&& other) = delete;

		GLint location = 0;
		GLenum type = 0;
		unsigned index = 0;
	};
}