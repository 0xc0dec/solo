#include "SoloPass.h"
#include "SoloEngine.h"
#include "SoloEffect.h"

using namespace solo;


Pass::Pass()
{
}


std::shared_ptr<Pass> Pass::create()
{
	return NEW2(Pass);
}


void Pass::setEffect(ptr<Effect> effect)
{
	_effect = effect;
}


void Pass::bind()
{
	if (_effect)
		_effect->bind();
}


void Pass::unbind()
{
}
