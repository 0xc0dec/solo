#pragma once

#include "SoloBase.h"
#include "SoloEffectVariable.h"
#include "SoloRenderState.h"

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
		friend Material;
		friend MaterialPass;
		friend RenderState;

	public:
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

		template <class TClass, class TParam>
		void bindValue(TClass* classInstance, TParam(TClass::*getValue)() const);

	private:
		MaterialParameter(const std::string &name);

		static ptr<MaterialParameter> create(const std::string &name);

		class ValueBinding
		{
		public:
			virtual ~ValueBinding() { }
			virtual void setValue(ptr<EffectVariable> variable) = 0;

		protected:
			ValueBinding& operator=(const ValueBinding&) { return *this; }
		};

		template <class TClass, class TParam>
		class SingleValueBinding : public ValueBinding
		{
			typedef TParam (TClass::*ValueGetterMethod)() const;

		public:
			SingleValueBinding(TClass* instance, ValueGetterMethod getter);

			virtual void setValue(ptr<EffectVariable> variable) override;

		private:
			TClass* _instance;
			ValueGetterMethod _getter;
		};

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
			MATRIX, // TODO
			METHOD
		} _type;

		union MaterialParameterValue
		{
			float asFloat;
			int asInt;
			float* asFloatPtr;
			int* asIntPtr;
			ValueBinding* method;
			explicit MaterialParameterValue(): asInt(0) {}
		} _value;

		unsigned _valueCount;
		bool _freeableValue;

		void bind(ptr<Effect> effect);

		void clearValue();
	};

	template <class TClass, class TParam>
	void MaterialParameter::SingleValueBinding<TClass, TParam>::setValue(ptr<EffectVariable> variable)
	{
		variable->setValue((_instance->*_getter)());
	}

	template <class TClass, class TParam>
	MaterialParameter::SingleValueBinding<TClass, TParam>::SingleValueBinding(TClass* instance, ValueGetterMethod getter):
		_instance(instance),
		_getter(getter)
	{
	}

	template <class TClass, class TParam>
	void MaterialParameter::bindValue(TClass* instance, TParam (TClass::*getter)() const)
	{
		clearValue();
		_value.method = new SingleValueBinding<TClass, TParam>(instance, getter);
		_freeableValue = true;
		_type = METHOD;
	}
}