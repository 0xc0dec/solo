#pragma once

#include "SoloBase.h"

namespace solo
{
	class Effect;
	class Material;
	class MaterialPass;

	class MaterialParameter
	{
	public:
		MaterialParameter(const std::string &name);
		~MaterialParameter() {}

		std::string getName() const;

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

		unsigned _valuesCount;

		void bind(ptr<Effect> effect);
	};
}