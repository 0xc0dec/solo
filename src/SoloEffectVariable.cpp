#include "SoloEffectVariable.h"

using namespace solo;


EffectVariable::EffectVariable(const std::string& name):
	_name(name)
{
}


std::string EffectVariable::getName() const
{
	return _name;
}