#include "SoloMaterialParameter.h"
#include "SoloEffect.h"
#include "SoloEffectVariable.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"

using namespace solo;


MaterialParameter::MaterialParameter(const std::string& name):
	_name(name),
	_type(NONE),
	_valueCount(0),
	_freeableValue(false)
{
}


std::string MaterialParameter::getName() const
{
	return _name;
}


void MaterialParameter::setValue(float value)
{
	clearValue();
	_value.asFloat = value;
	_type = FLOAT;
}


void MaterialParameter::setValue(const float* value, unsigned count)
{
	clearValue();
	_value.asFloatPtr = const_cast<float*>(value);
	_type = FLOAT_ARRAY;
}


void MaterialParameter::setValue(int value)
{
	clearValue();
	_value.asInt = value;
	_type = INT;
}


void MaterialParameter::setValue(const int* value, unsigned count)
{
	clearValue();
	_value.asIntPtr = const_cast<int*>(value);
	_type = INT_ARRAY;
}


void MaterialParameter::setValue(const Vector2& value)
{
	clearValue();
	auto buf = new float[2];
	memcpy(buf, &value.x, sizeof(float) * 2);
	_value.asFloatPtr = buf;
	_valueCount = 1;
	_freeableValue = true;
	_type = VECTOR2;
}


void MaterialParameter::setValue(const Vector2* value, unsigned count)
{
	clearValue();
	_value.asFloatPtr = const_cast<float*>(&value[0].x);
	_valueCount = count;
	_type = VECTOR2;
}


void MaterialParameter::setValue(const Vector3& value)
{
	clearValue();
	auto buf = new float[3];
	memcpy(buf, &value.x, sizeof(float) * 3);
	_value.asFloatPtr = buf;
	_valueCount = 1;
	_freeableValue = true;
	_type = VECTOR3;
}


void MaterialParameter::setValue(const Vector3* value, unsigned count)
{
	clearValue();
	_value.asFloatPtr = const_cast<float*>(&value[0].x);
	_valueCount = count;
	_type = VECTOR3;
}


void MaterialParameter::setValue(const Vector4& value)
{
	clearValue();
	auto buf = new float[4];
	memcpy(buf, &value.x, sizeof(float) * 4);
	_value.asFloatPtr = buf;
	_valueCount = 1;
	_freeableValue = true;
	_type = VECTOR4;
}


void MaterialParameter::setValue(const Vector4* value, unsigned count)
{
	clearValue();
	_value.asFloatPtr = const_cast<float*>(&value[0].x);
	_valueCount = count;
	_type = VECTOR4;
}


void MaterialParameter::bind(ptr<Effect> effect)
{
	auto variable = effect->findVariable(_name);
	if (variable)
	{
		switch (_type)
		{
			case FLOAT:
				variable->setValue(_value.asFloat);
				break;
			case FLOAT_ARRAY:
				variable->setValue(_value.asFloatPtr, _valueCount);
				break;
			case INT:
				variable->setValue(_value.asInt);
				break;
			case INT_ARRAY:
				variable->setValue(_value.asIntPtr, _valueCount);
				break;
			case VECTOR2:
				variable->setValue(reinterpret_cast<Vector2*>(_value.asFloatPtr), _valueCount);
				break;
			case VECTOR3:
				variable->setValue(reinterpret_cast<Vector3*>(_value.asFloatPtr), _valueCount);
				break;
			case VECTOR4:
				variable->setValue(reinterpret_cast<Vector4*>(_value.asFloatPtr), _valueCount);
				break;
			case METHOD:
				_value.method->setValue(variable);
			case NONE:
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
			case FLOAT:
			case FLOAT_ARRAY:
			case VECTOR2:
			case VECTOR3:
			case VECTOR4:
				delete[] _value.asFloatPtr;
				break;
			case INT:
			case INT_ARRAY:
				delete[] _value.asIntPtr;
				break;
			case METHOD:
				delete _value.method;
				break;
			case NONE:
			default:
				break;
		}
	}
	memset(&_value, 0, sizeof(_value));
	_type = NONE;
	_freeableValue = false;
	_valueCount = 1;
}
