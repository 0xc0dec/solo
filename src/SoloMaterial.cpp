#include "SoloMaterialParameter.h"
#include "SoloEffect.h"
#include "SoloRenderContext.h"
#include "SoloDevice.h"
#include "platform/stub/SoloStubMaterial.h"
#include "platform/opengl/SoloOpenGLMaterial.h"

using namespace solo;


shared<Material> Material::create(DeviceMode mode, shared<Effect> effect)
{
	if (mode == DeviceMode::OpenGL)
		return SL_NEW_SHARED(OpenGLMaterial, effect);
	return SL_NEW_SHARED(StubMaterial, effect);
}


Material::Material(shared<Effect> effect):
	effect(effect)
{
}


void Material::bind(RenderContext& context)
{
	applyState();
	if (effect)
	{
		effect->bind();
		for (auto p : parameters)
			p.second->apply(context);
	}
}


void Material::unbind(RenderContext& context)
{
	if (effect)
		effect->unbind();
}


MaterialParameter* Material::getParameter(const std::string& name)
{
	auto where = parameters.find(name);
	if (where != parameters.end())
		return where->second.get();
	auto parameter = SL_NEW_SHARED(MaterialParameter, name, this);
	parameters[name] = parameter;
	return parameter.get();
}
