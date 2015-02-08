#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloMaterialPass.h"
#include "SoloException.h"

using namespace solo;


ptr<Material> Material::create()
{
	return NEW2(Material);
}


ptr<MaterialPass> Material::addPass(ptr<Effect> effect)
{
	auto pass = MaterialPass::create(this, effect);
	passes.push_back(pass);
	return pass;
}


void Material::removePass(ptr<MaterialPass> pass)
{
	auto where = find(passes.begin(), passes.end(), pass);
	if (where != passes.end())
		passes.erase(where);
}


size_t Material::getPassCount() const
{
	return passes.size();
}


void Material::bind(ptr<Effect> effect, const RenderContext& context)
{
	for (auto p : parameters)
		p.second->bind(effect, context);
}


ptr<MaterialPass> Material::getPass(unsigned index) const
{
	return passes[index];
}
