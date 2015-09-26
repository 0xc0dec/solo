#pragma once

#include "SoloBase.h"

namespace solo
{
	struct Matrix;
	struct Vector2;
	struct Vector3;
	struct Vector4;
	class Texture;

	// Wrap around a uniform variable
	class EffectVariable
	{
	public:
		virtual ~EffectVariable() {}

		std::string getName() const;

		virtual void setFloat(float value) = 0;
		virtual void setFloatArray(const float *values, unsigned count) = 0;
		virtual void setInt(int value) = 0;
		virtual void setIntArray(const int *values, unsigned count) = 0;
		virtual void setMatrix(const Matrix& value) = 0;
		virtual void setMatrixArray(const Matrix* values, unsigned count) = 0;
		virtual void setVector2(const Vector2& value) = 0;
		virtual void setVector2Array(const Vector2* values, unsigned count) = 0;
		virtual void setVector3(const Vector3& value) = 0;
		virtual void setVector3Array(const Vector3* values, unsigned count) = 0;
		virtual void setVector4(const Vector4& value) = 0;
		virtual void setVector4Array(const Vector4* values, unsigned count) = 0;
		virtual void setTexture(const shared<Texture> texture) = 0;
		virtual void setTextureArray(const std::vector<shared<Texture>>& textures, unsigned count) = 0;

	protected:
		explicit EffectVariable(const std::string &name);

		std::string name;

	private:
		EffectVariable(const EffectVariable& other) = delete;
		EffectVariable(EffectVariable&& other) = delete;
		EffectVariable& operator=(const EffectVariable& other) = delete;
		EffectVariable& operator=(EffectVariable&& other) = delete;
	};
}