#include "SoloStubEffect.h"
#include "SoloStubEffectVariable.h"

using namespace solo;


StubEffect::StubEffect():
	variable(new StubEffectVariable("testVariable"))
{
}


EffectVariable* StubEffect::findVariable(const std::string& name) const
{
	return variable.get();
}