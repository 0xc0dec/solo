#include "SoloMaterialParameter.h"
#include "SoloEffect.h"
#include "SoloEffectVariable.h"

using namespace solo;


MaterialParameter::MaterialParameter(const std::string& name):
	_name(name),
	_type(NONE),
	_valuesCount(0)
{
}


std::string MaterialParameter::getName() const
{
	return _name;
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
				variable->setValue(_value.asFloatPtr, _valuesCount);
				break;
			case INT:
				variable->setValue(_value.asInt);
				break;
			case INT_ARRAY:
				variable->setValue(_value.asIntPtr, _valuesCount);
				break;
			case VECTOR2:
				variable->setValue(reinterpret_cast<Vector2*>(_value.asFloatPtr), _valuesCount);
				break;
			case VECTOR3:
				variable->setValue(reinterpret_cast<Vector3*>(_value.asFloatPtr), _valuesCount);
				break;
			case VECTOR4:
				variable->setValue(reinterpret_cast<Vector4*>(_value.asFloatPtr), _valuesCount);
				break;
			case NONE:
			default:
				break;
		}
	}
}