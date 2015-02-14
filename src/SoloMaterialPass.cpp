#include "SoloMaterialPass.h"
#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloEffect.h"

using namespace solo;


shared<MaterialPass> MaterialPassFactory::create(Material* material, shared<Effect> effect)
{
	return NEW2(MaterialPass, material, effect);
}


MaterialPass::MaterialPass(Material *material, shared<Effect> effect):
	material(material),
	effect(effect)
{
}


void MaterialPass::bind(const RenderContext& context)
{
	if (effect)
	{
		effect->bind();
		material->bind(effect.get(), context);
		for (auto p : parameters)
			p.second->bind(effect.get(), context);
	}
}


void MaterialPass::unbind()
{
}
