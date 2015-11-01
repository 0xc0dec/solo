#pragma once

#include "SoloBase.h"

namespace solo
{
	struct Matrix;
	struct Vector2;
	struct Vector3;
	struct Vector4;
	class Texture;

	class EffectVariable
	{
	public:
		EffectVariable(const EffectVariable& other) = delete;
		EffectVariable(EffectVariable&& other) = delete;
		EffectVariable& operator=(const EffectVariable& other) = delete;
		EffectVariable& operator=(EffectVariable&& other) = delete;
		virtual ~EffectVariable() {}

		std::string getName() const;

		virtual void setFloat(float value) = 0;
		virtual void setFloatArray(const std::vector<float>& value) = 0;
		virtual void setInt(int value) = 0;
		virtual void setIntArray(const std::vector<int>& value) = 0;
		virtual void setVector2(const Vector2 &value) = 0;
		virtual void setVector2Array(const std::vector<Vector2>& value) = 0;
		virtual void setVector3(const Vector3 &value) = 0;
		virtual void setVector3Array(const std::vector<Vector3>& value) = 0;
		virtual void setVector4(const Vector4 &value) = 0;
		virtual void setVector4Array(const std::vector<Vector4>& value) = 0;
		virtual void setMatrix(const Matrix &value) = 0;
		virtual void setMatrixArray(const std::vector<Matrix>& value) = 0;
		virtual void setTexture(const shared<Texture> texture) = 0;
		virtual void setTextureArray(const std::vector<shared<Texture>>& textures) = 0;

	protected:
		explicit EffectVariable(const std::string &name): name(name) {}

		std::string name;
	};

	inline std::string EffectVariable::getName() const
	{
		return name;
	}
}