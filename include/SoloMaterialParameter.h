#pragma once

#include "SoloBase.h"
#include "SoloEffectVariable.h"

namespace solo
{
	class Effect;
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Matrix;
	class Scene;
	struct RenderContext;
	class Texture;
	struct Quaternion;

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

	class MaterialParameter
	{
	public:
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
		void setValue(const shared<Texture> texture);
		void setValue(const std::vector<shared<Texture>>& textures, unsigned count);

		template <class TClass, class TValue>
		void bindValue(TClass* classInstance, TValue(TClass::*getValue)(const RenderContext& context) const);

		template <class TValue>
		void bindValue(std::function<TValue(const RenderContext& context)> lambda);

		void bindValue(AutoBinding autoBinding);

	private:
		friend class Material;
		friend class RenderState;

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
			Texture,
			TextureArray,
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
		class SingleValueBinding : public ValueBinding // TODO needed? Now that we have lambda function binding...
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
			MaterialParameterValue() : asInt(0) {}
		};

		MaterialParameter(const std::string &name);
		MaterialParameter(const MaterialParameter& other) = delete;
		MaterialParameter(MaterialParameter&& other) = delete;
		MaterialParameter& operator=(const MaterialParameter& other) = delete;
		MaterialParameter& operator=(MaterialParameter&& other) = delete;

		Scene* scene = nullptr;
		std::string name;
		ValueType type = ValueType::None;
		unsigned valueCount = 0;
		bool freeableValue = false;
		MaterialParameterValue value;
		shared<Texture> textureValue;
		std::vector<shared<Texture>> textureArrayValue;

		void apply(const RenderContext& context);

		const Matrix& getWorldMatrix(const RenderContext& context) const;
		const Matrix& getViewMatrix(const RenderContext& context) const;
		const Matrix& getProjectionMatrix(const RenderContext& context) const;
		Vector3 getCameraWorldPosition(const RenderContext& context) const;
		Matrix getWorldViewMatrix(const RenderContext& context) const;
		Matrix getViewProjectionMatrix(const RenderContext& context) const;
		Matrix getWorldViewProjectionMatrix(const RenderContext& context) const;
		Matrix getInverseTransposedWorldMatrix(const RenderContext& context) const;
		Matrix getInverseTransposedWorldViewMatrix(const RenderContext& context) const;
	};

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

	template <class TClass, class TParam>
	void MaterialParameter::SingleValueBinding<TClass, TParam>::setValue(EffectVariable* variable, const RenderContext& context)
	{
		variable->setValue((instance->*getter)(context));
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