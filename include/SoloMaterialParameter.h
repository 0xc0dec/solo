#pragma once

#include "SoloBase.h"
#include "SoloEffectVariable.h"
#include "SoloRenderState.h"

namespace solo
{
	class Effect;
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix;
	class Scene;
	class Transform;
	class Camera;
	struct RenderContext;

	class MaterialParameter
	{
		friend class Material;
		friend class MaterialPass;
		friend class RenderState;

	public:
		enum class AutoBinding
		{
			NONE,
			WORLD_MATRIX,
			VIEW_MATRIX,
			PROJECTION_MATRIX,
			WORLD_VIEW_MATRIX,
			VIEW_PROJECTION_MATRIX,
			WORLD_VIEW_PROJECTION_MATRIX,
			INVERSE_TRANSPOSE_WORLD_MATRIX,
			INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX,
			CAMERA_WORLD_POSITION,
			CAMERA_VIEW_POSITION,
		};

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
		void setValue(const Matrix &value);
		void setValue(const Matrix *value, unsigned count);

		template <class TClass, class TParam>
		void bindValue(TClass* classInstance, TParam(TClass::*getValue)() const);

		void bindValue(AutoBinding autoBinding);

	private:
		enum class ValueType
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
		};
		
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

		union MaterialParameterValue
		{
			float asFloat;
			int asInt;
			float* asFloatPtr;
			int* asIntPtr;
			ValueBinding* method;
			explicit MaterialParameterValue() : asInt(0) {}
		};

		MaterialParameter(const std::string &name);

		ptr<Scene> _scene;

		size_t _renderedNode;
		ptr<Transform> _renderedNodeTransform;
		ptr<Camera> _renderingCamera;

		std::string _name;
		ValueType _type;
		MaterialParameterValue _value;
		unsigned _valueCount;
		bool _freeableValue;

		static ptr<MaterialParameter> create(const std::string &name);

		void bind(ptr<Effect> effect, const RenderContext& context);
		void clearValue();

		const Matrix& getAutoBoundWorldMatrix() const;
		const Matrix& getAutoBoundViewMatrix() const;
		const Matrix& getAutoBoundProjectionMatrix() const;
		const Matrix& getAutoBoundWorldViewMatrix() const;
		const Matrix& getAutoBoundViewProjectionMatrix() const;
		const Matrix& getAutoBoundWorldViewProjectionMatrix() const;
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
		_type = ValueType::METHOD;
	}
}