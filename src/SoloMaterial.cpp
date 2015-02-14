#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloMaterialPass.h"

using namespace solo;


MaterialPass* Material::addPass(shared<Effect> effect)
{
	auto pass = MaterialPassFactory::create(this, effect);
	passes.push_back(pass);
	return pass.get();
}


void Material::removePass(MaterialPass* pass)
{
	auto where = std::find_if(passes.begin(), passes.end(),
		[pass](shared <MaterialPass> p) -> bool { return p.get() == pass; });
	if (where != passes.end())
		passes.erase(where);
}


size_t Material::getPassCount() const
{
	return passes.size();
}


void Material::bind(Effect* effect, const RenderContext& context)
{
	for (auto p : parameters)
		p.second->bind(effect, context);
}


MaterialPass* Material::getPass(unsigned index) const
{
	return passes[index].get();
}


shared <Material> MaterialFactory::create()
{
	return NEW2(Material);
}
