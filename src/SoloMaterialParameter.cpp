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
	_renderedNode(0),
	_name(name),
	_type(ValueType::NONE),
	_valueCount(0),
	_freeableValue(false)
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
	_value.asFloat = value;
	_type = ValueType::FLOAT;
}


void MaterialParameter::setValue(const float* value, unsigned count)
{
	clearValue();
	_value.asFloatPtr = const_cast<float*>(value);
	_valueCount = count;
	_type = ValueType::FLOAT_ARRAY;
}


void MaterialParameter::setValue(int value)
{
	clearValue();
	_value.asInt = value;
	_type = ValueType::INT;
}


void MaterialParameter::setValue(const int* value, unsigned count)
{
	clearValue();
	_value.asIntPtr = const_cast<int*>(value);
	_valueCount = count;
	_type = ValueType::INT_ARRAY;
}


void MaterialParameter::setValue(const Vector2& value)
{
	clearValue();
	auto buf = new float[2];
	memcpy(buf, &value.x, sizeof(float) * 2);
	_value.asFloatPtr = buf;
	_valueCount = 1;
	_freeableValue = true;
	_type = ValueType::VECTOR2;
}


void MaterialParameter::setValue(const Vector2* value, unsigned count)
{
	clearValue();
	_value.asFloatPtr = const_cast<float*>(&value[0].x);
	_valueCount = count;
	_type = ValueType::VECTOR2;
}


void MaterialParameter::setValue(const Vector3& value)
{
	clearValue();
	auto buf = new float[3];
	memcpy(buf, &value.x, sizeof(float) * 3);
	_value.asFloatPtr = buf;
	_valueCount = 1;
	_freeableValue = true;
	_type = ValueType::VECTOR3;
}


void MaterialParameter::setValue(const Vector3* value, unsigned count)
{
	clearValue();
	_value.asFloatPtr = const_cast<float*>(&value[0].x);
	_valueCount = count;
	_type = ValueType::VECTOR3;
}


void MaterialParameter::setValue(const Vector4& value)
{
	clearValue();
	auto buf = new float[4];
	memcpy(buf, &value.x, sizeof(float) * 4);
	_value.asFloatPtr = buf;
	_valueCount = 1;
	_freeableValue = true;
	_type = ValueType::VECTOR4;
}


void MaterialParameter::setValue(const Vector4* value, unsigned count)
{
	clearValue();
	_value.asFloatPtr = const_cast<float*>(&value[0].x);
	_valueCount = count;
	_type = ValueType::VECTOR4;
}


void MaterialParameter::setValue(const Matrix& value)
{
	clearValue(); // TODO eliminate memory deallocation for cases of second setting of a matrix
	_value.asFloatPtr = new float[16];
	memcpy(_value.asFloatPtr, value.m, sizeof(float)* 16);
	_freeableValue = true;
	_valueCount = 1;
	_type = ValueType::MATRIX;
}


void MaterialParameter::setValue(const Matrix* value, unsigned count)
{
	clearValue();
	_value.asFloatPtr = const_cast<Matrix&>(value[0]).m;
	_valueCount = count;
	_type = ValueType::MATRIX;
}


void MaterialParameter::bind(ptr<Effect> effect, const RenderContext& context)
{
	_renderedNode = context.renderedNode;
	_renderedNodeTransform = _scene->getComponent<Transform>(context.renderedNode);
	_renderingCamera = context.renderingCamera;
	auto variable = effect->findVariable(_name);
	if (variable)
	{
		switch (_type)
		{
			case ValueType::FLOAT:
				variable->setValue(_value.asFloat);
				break;
			case ValueType::FLOAT_ARRAY:
				variable->setValue(_value.asFloatPtr, _valueCount);
				break;
			case ValueType::INT:
				variable->setValue(_value.asInt);
				break;
			case ValueType::INT_ARRAY:
				variable->setValue(_value.asIntPtr, _valueCount);
				break;
			case ValueType::VECTOR2:
				variable->setValue(reinterpret_cast<Vector2*>(_value.asFloatPtr), _valueCount);
				break;
			case ValueType::VECTOR3:
				variable->setValue(reinterpret_cast<Vector3*>(_value.asFloatPtr), _valueCount);
				break;
			case ValueType::VECTOR4:
				variable->setValue(reinterpret_cast<Vector4*>(_value.asFloatPtr), _valueCount);
				break;
			case ValueType::MATRIX:
				variable->setValue(reinterpret_cast<Matrix*>(_value.asFloatPtr), _valueCount);
				break;
			case ValueType::METHOD:
				_value.method->setValue(variable);
			case ValueType::NONE:
			default:
				break;
		}
	}
}


void MaterialParameter::clearValue()
{
	if (_freeableValue)
	{
		switch (_type)
		{
			case ValueType::FLOAT:
			case ValueType::FLOAT_ARRAY:
			case ValueType::VECTOR2:
			case ValueType::VECTOR3:
			case ValueType::VECTOR4:
			case ValueType::MATRIX:
				delete[] _value.asFloatPtr;
				break;
			case ValueType::INT:
			case ValueType::INT_ARRAY:
				delete[] _value.asIntPtr;
				break;
			case ValueType::METHOD:
				delete _value.method;
				break;
			case ValueType::NONE:
			default:
				break;
		}
	}
	memset(&_value, 0, sizeof(_value));
	_freeableValue = false;
	_valueCount = 1;
	_type = ValueType::NONE;
}


const Matrix& MaterialParameter::getAutoBoundWorldMatrix() const
{
	return _renderedNodeTransform->getWorldMatrix();
}


const Matrix& MaterialParameter::getAutoBoundViewMatrix() const
{
	return _renderingCamera->getViewMatrix();
}


const Matrix& MaterialParameter::getAutoBoundProjectionMatrix() const
{
	return _renderingCamera->getProjectionMatrix();
}


const Matrix& MaterialParameter::getAutoBoundWorldViewMatrix() const
{
	return Matrix::identity();// TODO
}


void MaterialParameter::bindValue(AutoBinding autoBinding)
{
	switch (autoBinding)
	{
		case AutoBinding::WORLD_MATRIX:
			bindValue(this, &MaterialParameter::getAutoBoundWorldMatrix);
			break;
		case AutoBinding::VIEW_MATRIX:
			bindValue(this, &MaterialParameter::getAutoBoundViewMatrix);
			break;
		case AutoBinding::PROJECTION_MATRIX:
			bindValue(this, &MaterialParameter::getAutoBoundProjectionMatrix);
			break;
		case AutoBinding::WORLD_VIEW_MATRIX:
			bindValue(this, &MaterialParameter::getAutoBoundWorldViewMatrix);
			break;
		case AutoBinding::VIEW_PROJECTION_MATRIX:
			bindValue(this, &MaterialParameter::getAutoBoundViewProjectionMatrix);
			break;
		case AutoBinding::WORLD_VIEW_PROJECTION_MATRIX:
			bindValue(this, &MaterialParameter::getAutoBoundWorldViewProjectionMatrix);
			break;
		case AutoBinding::INVERSE_TRANSPOSE_WORLD_MATRIX:
			break;
		case AutoBinding::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX:
			break;
		case AutoBinding::CAMERA_WORLD_POSITION:
			break;
		case AutoBinding::CAMERA_VIEW_POSITION:
			break;
		case AutoBinding::NONE:
		default:
			break;
	}
}
