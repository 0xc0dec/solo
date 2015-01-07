#include "SoloMaterialPass.h"
#include "SoloEngine.h"
#include "SoloEffect.h"

using namespace solo;


MaterialPass::MaterialPass()
{
}


ptr<MaterialPass> MaterialPass::create()
{
	return NEW2(MaterialPass);
}


void MaterialPass::setEffect(ptr<Effect> effect)
{
	_effect = effect;
}


void MaterialPass::bind()
{
	if (_effect)
		_effect->bind();
}


void MaterialPass::unbind()
{
}
