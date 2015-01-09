#include "SoloMaterialPass.h"
#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloEffect.h"
#include "SoloRenderContext.h"

using namespace solo;


MaterialPass::MaterialPass(Material *material, ptr<Effect> effect):
	_material(material),
	_effect(effect)
{
}


ptr<MaterialPass> MaterialPass::create(Material* material, ptr<Effect> effect)
{
	return NEW2(MaterialPass, material, effect);
}


void MaterialPass::bind(const RenderContext& context)
{
	if (_effect)
	{
		_effect->bind();
		_material->bind(_effect, context);
		for (auto p : _parameters)
			p.second->bind(_effect, context);
	}
}


void MaterialPass::unbind()
{
}
