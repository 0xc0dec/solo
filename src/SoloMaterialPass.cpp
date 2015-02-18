#include "SoloMaterialPass.h"
#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloEffect.h"
#include "SoloRenderContext.h"

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


void MaterialPass::bind(RenderContext& context)
{
	if (effect && effect->isValid())
	{
		context.setPass(this);
		effect->bind();
		material->bind(context);
		for (auto p : parameters)
			p.second->apply(context);
	}
}


void MaterialPass::unbind(RenderContext& context)
{
	context.setPass(nullptr);
}


Effect* MaterialPass::getEffect()
{
	return effect.get();
}