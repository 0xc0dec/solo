#include "SoloMaterialParameter.h"
#include "SoloTransform.h"
#include "SoloEffect.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloMatrix.h"
#include "SoloScene.h"
#include "SoloEngine.h"
#include "SoloRenderContext.h"

using namespace solo;


MaterialParameter::MaterialParameter(const std::string& name):
	_name(name),
	type(ValueType::NONE),
	_valueCount(0),
	freeableValue(false)
{
	_scene = Engine::get()->getScene();
}


ptr<MaterialParameter> MaterialParameter::create(const std::string& name)
{
	return NEW2(MaterialParameter, name);
}


std::string MaterialParameter::getName() const
{
	return _name;
}


void MaterialParameter::setValue(float value)
{
	clearValue();
	this->value.asFloat = value;
	type = ValueType::FLOAT;
}


void MaterialParameter::setValue(const float* value, unsigned count)
{
	clearValue();
	this->value.asFloatPtr = const_cast<float*>(value);
	_valueCount = count;
	type = ValueType::FLOAT_ARRAY;
}


void MaterialParameter::setValue(int value)
{
	clearValue();
	this->value.asInt = value;
	type = ValueType::INT;
}


void MaterialParameter::setValue(const int* value, unsigned count)
{
	clearValue();
	this->value.asIntPtr = const_cast<int*>(value);
	_valueCount = count;
	type = ValueType::INT_ARRAY;
}


void MaterialParameter::setValue(const Vector2& value)
{
	clearValue();
	auto buf = new float[2];
	memcpy(buf, &value.x, sizeof(float) * 2);
	this->value.asFloatPtr = buf;
	_valueCount = 1;
	freeableValue = true;
	type = ValueType::VECTOR2;
}


void MaterialParameter::setValue(const Vector2* value, unsigned count)
{
	clearValue();
	this->value.asFloatPtr = const_cast<float*>(&value[0].x);
	_valueCount = count;
	type = ValueType::VECTOR2;
}


void MaterialParameter::setValue(const Vector3& value)
{
	clearValue();
	auto buf = new float[3];
	memcpy(buf, &value.x, sizeof(float) * 3);
	this->value.asFloatPtr = buf;
	_valueCount = 1;
	freeableValue = true;
	type = ValueType::VECTOR3;
}


void MaterialParameter::setValue(const Vector3* value, unsigned count)
{
	clearValue();
	this->value.asFloatPtr = const_cast<float*>(&value[0].x);
	_valueCount = count;
	type = ValueType::VECTOR3;
}


void MaterialParameter::setValue(const Vector4& value)
{
	clearValue();
	auto buf = new float[4];
	memcpy(buf, &value.x, sizeof(float) * 4);
	this->value.asFloatPtr = buf;
	_valueCount = 1;
	freeableValue = true;
	type = ValueType::VECTOR4;
}


void MaterialParameter::setValue(const Vector4* value, unsigned count)
{
	clearValue();
	this->value.asFloatPtr = const_cast<float*>(&value[0].x);
	_valueCount = count;
	type = ValueType::VECTOR4;
}


void MaterialParameter::setValue(const Matrix& value)
{
	clearValue(); // TODO eliminate memory deallocation for cases of second setting of a matrix
	this->value.asFloatPtr = new float[16];
	memcpy(this->value.asFloatPtr, value.m, sizeof(float)* 16);
	freeableValue = true;
	_valueCount = 1;
	type = ValueType::MATRIX;
}


void MaterialParameter::setValue(const Matrix* value, unsigned count)
{
	clearValue();
	this->value.asFloatPtr = const_cast<Matrix&>(value[0]).m;
	_valueCount = count;
	type = ValueType::MATRIX;
}


void MaterialParameter::bind(ptr<Effect> effect, const RenderContext& context)
{
	auto variable = effect->findVariable(_name);
	if (variable)
	{
		switch (type)
		{
			case ValueType::FLOAT:
				variable->setValue(value.asFloat);
				break;
			case ValueType::FLOAT_ARRAY:
				variable->setValue(value.asFloatPtr, _valueCount);
				break;
			case ValueType::INT:
				variable->setValue(value.asInt);
				break;
			case ValueType::INT_ARRAY:
				variable->setValue(value.asIntPtr, _valueCount);
				break;
			case ValueType::VECTOR2:
				variable->setValue(reinterpret_cast<Vector2*>(value.asFloatPtr), _valueCount);
				break;
			case ValueType::VECTOR3:
				variable->setValue(reinterpret_cast<Vector3*>(value.asFloatPtr), _valueCount);
				break;
			case ValueType::VECTOR4:
				variable->setValue(reinterpret_cast<Vector4*>(value.asFloatPtr), _valueCount);
				break;
			case ValueType::MATRIX:
				variable->setValue(reinterpret_cast<Matrix*>(value.asFloatPtr), _valueCount);
				break;
			case ValueType::METHOD:
				value.method->setValue(variable, context);
			case ValueType::NONE:
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
			case ValueType::FLOAT:
			case ValueType::FLOAT_ARRAY:
			case ValueType::VECTOR2:
			case ValueType::VECTOR3:
			case ValueType::VECTOR4:
			case ValueType::MATRIX:
				delete[] value.asFloatPtr;
				break;
			case ValueType::INT:
			case ValueType::INT_ARRAY:
				delete[] value.asIntPtr;
				break;
			case ValueType::METHOD:
				delete value.method;
				break;
			case ValueType::NONE:
			default:
				break;
		}
	}
	memset(&value, 0, sizeof(value));
	freeableValue = false;
	_valueCount = 1;
	type = ValueType::NONE;
}


const Matrix& MaterialParameter::getWorldMatrix(const RenderContext& context) const
{
	return context.getNodeTransform()->getWorldMatrix();
}


const Matrix& MaterialParameter::getViewMatrix(const RenderContext& context) const
{
	return context.getCamera()->getViewMatrix();
}


const Matrix& MaterialParameter::getProjectionMatrix(const RenderContext& context) const
{
	return context.getCamera()->getProjectionMatrix();
}


Matrix MaterialParameter::getWorldViewMatrix(const RenderContext& context) const
{
	return context.getNodeTransform()->getWorldViewMatrix(context.getCamera());
}


Matrix MaterialParameter::getViewProjectionMatrix(const RenderContext& context) const
{
	return context.getCamera()->getViewProjectionMatrix();
}


Matrix MaterialParameter::getWorldViewProjectionMatrix(const RenderContext& context) const
{
	return context.getNodeTransform()->getWorldViewProjectionMatrix(context.getCamera());
}


Matrix MaterialParameter::getInverseTransposedWorldViewMatrix(const RenderContext& context) const
{
	return context.getNodeTransform()->getInverseTransposedWorldViewMatrix(context.getCamera());
}


const Matrix& MaterialParameter::getInverseTransposedWorldMatrix(const RenderContext& context) const
{
	return context.getNodeTransform()->getInverseTransposedWorldMatrix();
}


Vector3 MaterialParameter::getCameraWorldPosition(const RenderContext& context) const
{
	return context.getCameraTransform()->getWorldPosition();
}


void MaterialParameter::bindValue(AutoBinding autoBinding)
{
	switch (autoBinding)
	{
		case AutoBinding::WORLD_MATRIX:
			bindValue(this, &MaterialParameter::getWorldMatrix);
			break;
		case AutoBinding::VIEW_MATRIX:
			bindValue(this, &MaterialParameter::getViewMatrix);
			break;
		case AutoBinding::PROJECTION_MATRIX:
			bindValue(this, &MaterialParameter::getProjectionMatrix);
			break;
		case AutoBinding::WORLD_VIEW_MATRIX:
			bindValue(this, &MaterialParameter::getWorldViewMatrix);
			break;
		case AutoBinding::VIEW_PROJECTION_MATRIX:
			bindValue(this, &MaterialParameter::getViewProjectionMatrix);
			break;
		case AutoBinding::WORLD_VIEW_PROJECTION_MATRIX:
			bindValue(this, &MaterialParameter::getWorldViewProjectionMatrix);
			break;
		case AutoBinding::INVERSE_TRANSPOSED_WORLD_MATRIX:
			bindValue(this, &MaterialParameter::getInverseTransposedWorldMatrix);
			break;
		case AutoBinding::INVERSE_TRANSPOSED_WORLD_VIEW_MATRIX:
			bindValue(this, &MaterialParameter::getInverseTransposedWorldViewMatrix);
			break;
		case AutoBinding::CAMERA_WORLD_POSITION:
			bindValue(this, &MaterialParameter::getCameraWorldPosition);
			break;
		case AutoBinding::NONE:
		default:
			break;
	}
}
