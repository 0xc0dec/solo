#include "SoloStubMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloEffect.h"
#include "SoloRenderContext.h"
#include "SoloEngineCreationArgs.h"
#include "platform/SoloOpenGLMaterial.h"

using namespace solo;


shared<Material> MaterialFactory::create(EngineMode mode, shared<Effect> effect)
{
	if (mode == EngineMode::OpenGL)
		return NEW2(OpenGLMaterial, effect);
	return NEW2(StubMaterial, effect);
}


Material::Material(shared<Effect> effect):
	effect(effect)
{
}


void Material::bind(RenderContext& context)
{
	context.material = this;
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
	if (context.material == this)
		context.material = nullptr;
}


MaterialParameter* Material::getParameter(const std::string& name)
{
	auto where = parameters.find(name);
	if (where != parameters.end())
		return where->second.get();
	auto parameter = NEW2(MaterialParameter, name);
	parameters[name] = parameter;
	return parameter.get();
}


Effect* Material::getEffect() const
{
	return effect.get();
}