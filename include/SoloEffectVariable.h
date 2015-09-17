#pragma once

#include "SoloBase.h"

namespace solo
{
	class Matrix;
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

		virtual void setValue(float value) = 0;
		virtual void setValue(const float *values, unsigned count) = 0;
		virtual void setValue(int value) = 0;
		virtual void setValue(const int *values, unsigned count) = 0;
		virtual void setValue(const Matrix& value) = 0;
		virtual void setValue(const Matrix* values, unsigned count) = 0;
		virtual void setValue(const Vector2& value) = 0;
		virtual void setValue(const Vector2* values, unsigned count) = 0;
		virtual void setValue(const Vector3& value) = 0;
		virtual void setValue(const Vector3* values, unsigned count) = 0;
		virtual void setValue(const Vector4& value) = 0;
		virtual void setValue(const Vector4* values, unsigned count) = 0;
		virtual void setValue(const shared<Texture> texture) = 0;
		virtual void setValue(const std::vector<shared<Texture>>& textures, unsigned count) = 0;

	protected:
		EffectVariable(const std::string &name);

		std::string name;
	};
}