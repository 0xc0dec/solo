#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloEffect.h"
#include "SoloRenderContext.h"
#include "platform/SoloOpenGLMaterial.h"

using namespace solo;


Material::Material(shared<Effect> effect):
	effect(effect)
{
}


shared <Material> MaterialFactory::create(shared<Effect> effect)
{
	return NEW2(OpenGLMaterial, effect);
}


void Material::bind(RenderContext& context)
{
	context.setMaterial(this);
	applyFaceCull();
	if (effect)
	{
		effect->bind();
		for (auto p : parameters)
			p.second->apply(context);
	}
}


void Material::unbind(RenderContext& context)
{
	context.setMaterial(nullptr);
}


Effect* Material::getEffect() const
{
	return effect.get();
}