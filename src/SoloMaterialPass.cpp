#include "SoloMaterialPass.h"
#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloEffect.h"

using namespace solo;


MaterialPass::MaterialPass(Material *material, ptr<Effect> effect):
	material(material),
	effect(effect)
{
}


ptr<MaterialPass> MaterialPass::create(Material* material, ptr<Effect> effect)
{
	return NEW2(MaterialPass, material, effect);
}


void MaterialPass::bind(const RenderContext& context)
{
	if (effect)
	{
		effect->bind();
		material->bind(effect, context);
		for (auto p : parameters)
			p.second->bind(effect, context);
	}
}


void MaterialPass::unbind()
{
}
