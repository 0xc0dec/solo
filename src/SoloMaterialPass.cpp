#include "SoloMaterialPass.h"
#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloEffect.h"

using namespace solo;


MaterialPass::MaterialPass(Material *material, ptr<Effect> effect):
	_material(material),
	_effect(effect)
{
}


void MaterialPass::bind()
{
	if (_effect)
	{
		_effect->bind();
		_material->bind(_effect);
		for (auto p : _parameters)
			p.second->bind(_effect);
	}
}


void MaterialPass::unbind()
{
}
