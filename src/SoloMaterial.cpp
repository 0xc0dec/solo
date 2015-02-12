#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloMaterialPass.h"

using namespace solo;


MaterialPass* Material::addPass(Effect* effect)
{
	auto pass = NEW2(MaterialPass, this, effect);
	passes.push_back(pass);
	return pass.get();
}


void Material::removePass(MaterialPass* pass)
{
	auto where = std::find_if(passes.begin(), passes.end(),
		[pass](ptr<MaterialPass> p) -> bool { return p.get() == pass; });
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


ptr<Material> MaterialFactory::create()
{
	return NEW2(Material);
}
