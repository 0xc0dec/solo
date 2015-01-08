#pragma once

#include "SoloBase.h"

namespace solo
{
	class Effect;
	class Material;
	class MaterialPass;
	class Vector2;
	class Vector3;
	class Vector4;

	class MaterialParameter
	{
	public:
		MaterialParameter(const std::string &name);
		~MaterialParameter() {}

		std::string getName() const;

		void setValue(float value);
		void setValue(const float *value, unsigned count);
		void setValue(int value);
		void setValue(const int *value, unsigned count);
		void setValue(const Vector2 &value);
		void setValue(const Vector2 *value, unsigned count);
		void setValue(const Vector3 &value);
		void setValue(const Vector3 *value, unsigned count);
		void setValue(const Vector4 &value);
		void setValue(const Vector4 *value, unsigned count);

	private:
		friend Material;
		friend MaterialPass;

		std::string _name;

		enum
		{
			NONE = 0,
			FLOAT,
			FLOAT_ARRAY,
			INT,
			INT_ARRAY,
			VECTOR2,
			VECTOR3,
			VECTOR4,
			MATRIX,
			SAMPLER,
			SAMPLER_ARRAY,
			METHOD
		} _type;

		union MaterialParameterValue
		{
			float asFloat;
			int asInt;
			float* asFloatPtr;
			int* asIntPtr;
			explicit MaterialParameterValue(): asInt(0) {}
		} _value;

		unsigned _valueCount;
		bool _freeableValue;

		void bind(ptr<Effect> effect);

		void clearValue();
	};
}