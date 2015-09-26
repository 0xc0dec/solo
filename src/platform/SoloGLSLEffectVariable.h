#pragma once

#include <GL/glew.h>
#include "SoloBase.h"
#include "SoloEffectVariable.h"

namespace solo
{
	class GLSLEffectVariable : public EffectVariable
	{
	public:
		virtual void setFloat(float value) override;
		virtual void setFloatArray(const float* values, unsigned count) override;
		virtual void setInt(int value) override;
		virtual void setIntArray(const int* values, unsigned count) override;
		virtual void setMatrix(const Matrix& value) override;
		virtual void setMatrixArray(const Matrix* values, unsigned count) override;
		virtual void setVector2(const Vector2& value) override;
		virtual void setVector2Array(const Vector2* values, unsigned count) override;
		virtual void setVector3(const Vector3& value) override;
		virtual void setVector3Array(const Vector3* values, unsigned count) override;
		virtual void setVector4(const Vector4& value) override;
		virtual void setVector4Array(const Vector4* values, unsigned count) override;
		virtual void setTexture(const shared<Texture> sampler) override;
		virtual void setTextureArray(const std::vector<shared<Texture>>& samplers, unsigned count) override;

	private:
		friend class GLSLEffect;

		GLSLEffectVariable(const std::string& name, GLint location, GLenum type, unsigned index);

		GLint location = 0;
		GLenum type = 0;
		unsigned index = 0;
	};
}