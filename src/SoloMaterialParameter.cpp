#include "SoloMaterialParameter.h"
#include "SoloTransform.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloMatrix.h"
#include "SoloEngine.h"
#include "SoloRenderContext.h"
#include "SoloNode.h"
#include "SoloMaterialPass.h"
#include "SoloEffect.h"

using namespace solo;


MaterialParameter::MaterialParameter(const std::string& name):
	name(name),
	type(ValueType::None),
	valueCount(0),
	freeableValue(false)
{
	scene = Engine::get()->getScene();
}


void MaterialParameter::setValue(float value)
{
	clearValue();
	this->value.asFloat = value;
	type = ValueType::Float;
}


void MaterialParameter::setValue(const float* value, unsigned count)
{
	clearValue();
	this->value.asFloatPtr = const_cast<float*>(value);
	valueCount = count;
	type = ValueType::FloatArray;
}


void MaterialParameter::setValue(int value)
{
	clearValue();
	this->value.asInt = value;
	type = ValueType::Int;
}


void MaterialParameter::setValue(const int* value, unsigned count)
{
	clearValue();
	this->value.asIntPtr = const_cast<int*>(value);
	valueCount = count;
	type = ValueType::IntArray;
}


void MaterialParameter::setValue(const Vector2& value)
{
	clearValue(); // TODO eliminate memory deallocation for the case when setting a value of the same type twice
	auto buf = new float[2];
	memcpy(buf, &value.x, sizeof(float) * 2);
	this->value.asFloatPtr = buf;
	valueCount = 1;
	freeableValue = true;
	type = ValueType::Vector2;
}


void MaterialParameter::setValue(const Vector2* value, unsigned count)
{
	clearValue();
	this->value.asFloatPtr = const_cast<float*>(&value[0].x);
	valueCount = count;
	type = ValueType::Vector2;
}


void MaterialParameter::setValue(const Vector3& value)
{
	clearValue(); // TODO eliminate memory deallocation for the case when setting a value of the same type twice
	auto buf = new float[3];
	memcpy(buf, &value.x, sizeof(float) * 3);
	this->value.asFloatPtr = buf;
	valueCount = 1;
	freeableValue = true;
	type = ValueType::Vector3;
}


void MaterialParameter::setValue(const Vector3* value, unsigned count)
{
	clearValue();
	this->value.asFloatPtr = const_cast<float*>(&value[0].x);
	valueCount = count;
	type = ValueType::Vector3;
}


void MaterialParameter::setValue(const Vector4& value)
{
	clearValue();
	auto buf = new float[4];
	memcpy(buf, &value.x, sizeof(float) * 4);
	this->value.asFloatPtr = buf;
	valueCount = 1;
	freeableValue = true;
	type = ValueType::Vector4;
}


void MaterialParameter::setValue(const Vector4* value, unsigned count)
{
	clearValue();
	this->value.asFloatPtr = const_cast<float*>(&value[0].x);
	valueCount = count;
	type = ValueType::Vector4;
}


void MaterialParameter::setValue(const Matrix& value)
{
	clearValue(); // TODO eliminate memory deallocation for the case when setting a value of the same type twice
	this->value.asFloatPtr = new float[16];
	memcpy(this->value.asFloatPtr, value.m, sizeof(float)* 16);
	freeableValue = true;
	valueCount = 1;
	type = ValueType::Matrix;
}


void MaterialParameter::setValue(const Matrix* value, unsigned count)
{
	clearValue();
	this->value.asFloatPtr = const_cast<Matrix&>(value[0]).m;
	valueCount = count;
	type = ValueType::Matrix;
}


void MaterialParameter::setValue(const shared<TextureSampler> sampler)
{
	clearValue();
	samplerValue = sampler;
	valueCount = 1;
	type = ValueType::Sampler;
	freeableValue = true;
}


void MaterialParameter::setValue(const std::vector<shared<TextureSampler>>& samplers, unsigned count)
{
	clearValue();
	samplerArrayValue = samplers;
	valueCount = count;
	type = ValueType::SamplerArray;
	freeableValue = true;
}


void MaterialParameter::apply(RenderContext& context)
{
	auto variable = context.getPass()->getEffect()->findVariable(name);
	if (variable)
	{
		switch (type)
		{
			case ValueType::Float:
				variable->setValue(value.asFloat);
				break;
			case ValueType::FloatArray:
				variable->setValue(value.asFloatPtr, valueCount);
				break;
			case ValueType::Int:
				variable->setValue(value.asInt);
				break;
			case ValueType::IntArray:
				variable->setValue(value.asIntPtr, valueCount);
				break;
			case ValueType::Vector2:
				variable->setValue(reinterpret_cast<Vector2*>(value.asFloatPtr), valueCount);
				break;
			case ValueType::Vector3:
				variable->setValue(reinterpret_cast<Vector3*>(value.asFloatPtr), valueCount);
				break;
			case ValueType::Vector4:
				variable->setValue(reinterpret_cast<Vector4*>(value.asFloatPtr), valueCount);
				break;
			case ValueType::Matrix:
				variable->setValue(reinterpret_cast<Matrix*>(value.asFloatPtr), valueCount);
				break;
			case ValueType::Sampler:
				variable->setValue(samplerValue);
				break;
			case ValueType::SamplerArray:
				variable->setValue(samplerArrayValue, valueCount);
				break;
			case ValueType::Method:
				value.method->setValue(variable, context);
				break;
			case ValueType::None:
			default:
				break;
		}
	}
}


void MaterialParameter::clearValue()
{
	if (freeableValue)
	{
		switch (type)
		{
			case ValueType::Float:
			case ValueType::FloatArray:
			case ValueType::Vector2:
			case ValueType::Vector3:
			case ValueType::Vector4:
			case ValueType::Matrix:
				delete[] value.asFloatPtr;
				break;
			case ValueType::Int:
			case ValueType::IntArray:
				delete[] value.asIntPtr;
				break;
			case ValueType::Sampler:
				samplerValue = nullptr;
				break;
			case ValueType::SamplerArray:
				samplerArrayValue.clear();
				break;
			case ValueType::Method:
				delete value.method;
				break;
			case ValueType::None:
			default:
				break;
		}
	}
	memset(&value, 0, sizeof(value));
	freeableValue = false;
	valueCount = 1;
	type = ValueType::None;
}


const Matrix& MaterialParameter::getWorldMatrix(const RenderContext& context) const
{
	return context.getNode()->getComponent<Transform>()->getWorldMatrix();
}


const Matrix& MaterialParameter::getViewMatrix(const RenderContext& context) const
{
	return context.getCameraNode()->getComponent<Camera>()->getViewMatrix();
}


const Matrix& MaterialParameter::getProjectionMatrix(const RenderContext& context) const
{
	return context.getCameraNode()->getComponent<Camera>()->getProjectionMatrix();
}


Matrix MaterialParameter::getWorldViewMatrix(const RenderContext& context) const
{
	return context.getNode()->getComponent<Transform>()->getWorldViewMatrix(context.getCameraNode()->getComponent<Camera>());
}


Matrix MaterialParameter::getViewProjectionMatrix(const RenderContext& context) const
{
	return context.getCameraNode()->getComponent<Camera>()->getViewProjectionMatrix();
}


Matrix MaterialParameter::getWorldViewProjectionMatrix(const RenderContext& context) const
{
	return context.getNode()->getComponent<Transform>()->getWorldViewProjectionMatrix(context.getCameraNode()->getComponent<Camera>());
}


Matrix MaterialParameter::getInverseTransposedWorldViewMatrix(const RenderContext& context) const
{
	return context.getNode()->getComponent<Transform>()->getInverseTransposedWorldViewMatrix(context.getCameraNode()->getComponent<Camera>());
}


const Matrix& MaterialParameter::getInverseTransposedWorldMatrix(const RenderContext& context) const
{
	return context.getNode()->getComponent<Transform>()->getInverseTransposedWorldMatrix();
}


Vector3 MaterialParameter::getCameraWorldPosition(const RenderContext& context) const
{
	return context.getCameraNode()->getComponent<Transform>()->getWorldPosition();
}


void MaterialParameter::bindValue(AutoBinding autoBinding)
{
	switch (autoBinding)
	{
		case AutoBinding::WorldMatrix:
			bindValue(this, &MaterialParameter::getWorldMatrix);
			break;
		case AutoBinding::ViewMatrix:
			bindValue(this, &MaterialParameter::getViewMatrix);
			break;
		case AutoBinding::ProjectionMatrix:
			bindValue(this, &MaterialParameter::getProjectionMatrix);
			break;
		case AutoBinding::WorldViewProjection:
			bindValue(this, &MaterialParameter::getWorldViewMatrix);
			break;
		case AutoBinding::ViewProjectionMatrix:
			bindValue(this, &MaterialParameter::getViewProjectionMatrix);
			break;
		case AutoBinding::WorldViewProjectionMatrix:
			bindValue(this, &MaterialParameter::getWorldViewProjectionMatrix);
			break;
		case AutoBinding::InverseTransposedWorldMatrix:
			bindValue(this, &MaterialParameter::getInverseTransposedWorldMatrix);
			break;
		case AutoBinding::InverseTransposedWorldViewMatrix:
			bindValue(this, &MaterialParameter::getInverseTransposedWorldViewMatrix);
			break;
		case AutoBinding::CameraWorldPosition:
			bindValue(this, &MaterialParameter::getCameraWorldPosition);
			break;
		case AutoBinding::None:
		default:
			break;
	}
}
