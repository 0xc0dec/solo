#include "SoloEffectVariable.h"

using namespace solo;


EffectVariable::EffectVariable(const std::string& name):
	name(name)
{
}


std::string EffectVariable::getName() const
{
	return name;
}