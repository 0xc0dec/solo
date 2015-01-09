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
	_passes.push_back(pass);
	return pass;
}


void Material::removePass(ptr<MaterialPass> pass)
{
	auto where = find(_passes.begin(), _passes.end(), pass);
	if (where != _passes.end())
		_passes.erase(where);
}


size_t Material::getPassCount() const
{
	return _passes.size();
}


void Material::bind(ptr<Effect> effect, size_t node)
{
	for (auto p : _parameters)
		p.second->bind(effect, node);
}


ptr<MaterialPass> Material::getPass(unsigned index) const
{
	return _passes[index];
}
