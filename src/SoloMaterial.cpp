#include "SoloMaterial.h"
#include "SoloException.h"

using namespace solo;


ptr<Material> Material::create()
{
	return NEW2(Material);
}


void Material::addPass(ptr<MaterialPass> pass)
{
	_passes.push_back(pass);
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


ptr<MaterialPass> Material::getPass(unsigned index) const
{
	return _passes[index];
}


ptr<MaterialParameter> Material::findParameter(const std::string& name)
{
	auto where = _parameters.find(name);
	return where != _parameters.end() ? where->second : nullptr;
}


void Material::bind()
{
}