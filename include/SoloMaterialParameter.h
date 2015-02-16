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
	class RenderContext;

	class MaterialParameter
	{
		friend class Material;
		friend class MaterialPass;
		friend class RenderState;

	public:
		enum class AutoBinding
		{
			None,
			WorldMatrix,
			ViewMatrix,
			ProjectionMatrix,
			WorldViewProjection,
			ViewProjectionMatrix,
			WorldViewProjectionMatrix,
			InverseTransposedWorldMatrix,
			InverseTransposedWorldViewMatrix,
			CameraWorldPosition,
		};

		~MaterialParameter() {}

		std::string getName() const;

		void clearValue();

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

		template <class TClass, class TValue>
		void bindValue(TClass* classInstance, TValue(TClass::*getValue)(const RenderContext& context) const);

		template <class TValue>
		void bindValue(std::function<TValue(const RenderContext& context)> lambda);

		void bindValue(AutoBinding autoBinding);

	private:
		enum class ValueType
		{
			None = 0,
			Float,
			FloatArray,
			Int,
			IntArray,
			Vector2,
			Vector3,
			Vector4,
			Matrix,
			Method
		};
		
		class ValueBinding
		{
		public:
			virtual ~ValueBinding() { }
			virtual void setValue(EffectVariable* variable, const RenderContext& context) = 0;

		protected:
			ValueBinding& operator=(const ValueBinding&) { return *this; }
		};

		template <class TValue>
		class LambdaValueBinding : public ValueBinding
		{
		public:
			LambdaValueBinding(std::function<TValue(const RenderContext& context)> lambda);

			virtual void setValue(EffectVariable* variable, const RenderContext& context) override;

		private:
			std::function<TValue(const RenderContext& context)> lambda;
		};

		template <class TClass, class TParam>
		class SingleValueBinding : public ValueBinding
		{
			typedef TParam(TClass::*ValueGetterMethod)(const RenderContext& context) const;

		public:
			SingleValueBinding(TClass* instance, ValueGetterMethod getter);

			virtual void setValue(EffectVariable* variable, const RenderContext& renderContext) override;

		private:
			TClass* instance;
			ValueGetterMethod getter;
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
		MaterialParameter(const MaterialParameter& other) = delete;
		MaterialParameter(MaterialParameter&& other) = delete;
		MaterialParameter& operator=(const MaterialParameter& other) = delete;
		MaterialParameter& operator=(MaterialParameter&& other) = delete;

		Scene* scene;

		std::string name;
		ValueType type;
		MaterialParameterValue value;
		unsigned valueCount;
		bool freeableValue;

		void bind(Effect* effect, const RenderContext& context);

		const Matrix& getWorldMatrix(const RenderContext& context) const;
		const Matrix& getViewMatrix(const RenderContext& context) const;
		const Matrix& getProjectionMatrix(const RenderContext& context) const;
		const Matrix& getInverseTransposedWorldMatrix(const RenderContext& context) const;
		Vector3 getCameraWorldPosition(const RenderContext& context) const;
		Matrix getWorldViewMatrix(const RenderContext& context) const;
		Matrix getViewProjectionMatrix(const RenderContext& context) const;
		Matrix getWorldViewProjectionMatrix(const RenderContext& context) const;
		Matrix getInverseTransposedWorldViewMatrix(const RenderContext& context) const;
	};

	template <class TClass, class TParam>
	void MaterialParameter::SingleValueBinding<TClass, TParam>::setValue(EffectVariable* variable, const RenderContext& context)
	{
		variable->setValue((instance->*getter)(context));
	}

	template <class TClass, class TParam>
	MaterialParameter::SingleValueBinding<TClass, TParam>::SingleValueBinding(TClass* instance, ValueGetterMethod getter):
		instance(instance),
		getter(getter)
	{
	}

	template <class TValue>
	MaterialParameter::LambdaValueBinding<TValue>::LambdaValueBinding(std::function<TValue(const RenderContext& context)> lambda) :
		lambda(lambda)
	{
	}

	template <class TValue>
	void MaterialParameter::LambdaValueBinding<TValue>::setValue(EffectVariable* variable, const RenderContext& context)
	{
		variable->setValue(lambda(context));
	}

	template <class TClass, class TParam>
	void MaterialParameter::bindValue(TClass* instance, TParam(TClass::*getter)(const RenderContext& context) const)
	{
		clearValue();
		value.method = new SingleValueBinding<TClass, TParam>(instance, getter);
		freeableValue = true;
		type = ValueType::Method;
	}

	template <class TValue>
	void MaterialParameter::bindValue(std::function<TValue(const RenderContext& context)> lambda)
	{
		clearValue();
		value.method = new LambdaValueBinding<TValue>(lambda);
		freeableValue = true;
		type = ValueType::Method;
	}
}