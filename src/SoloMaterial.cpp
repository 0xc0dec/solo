#include "SoloMaterial.h"
#include "SoloException.h"

using namespace solo;


ptr<Material> Material::create()
{
	return NEW2(Material);
}


void Material::addTechnique(ptr<Technique> technique)
{
	_techniques.push_back(technique);
	if (!_currentTechnique)
		_currentTechnique = technique;
}


void Material::removeTechnique(ptr<Technique> technique)
{
	auto where = find(_techniques.begin(), _techniques.end(), technique);
	if (where != _techniques.end())
		_techniques.erase(where);
}


void Material::setTechnique(ptr<Technique> technique)
{
	auto where = find(_techniques.begin(), _techniques.end(), technique);
	if (where == _techniques.end())
		THROW(EngineException, "Material doesn't contain the specified technique");
	_currentTechnique = technique;
}


size_t Material::getTechniquesCount() const
{
	return _techniques.size();
}


ptr<Technique> Material::getTechnique(unsigned index) const
{
	// No bounds checks intentionally. It's user's responsibility to a pass correct index.
	return _techniques[index];
}


ptr<Technique> Material::getCurrentTechnique() const
{
	return _currentTechnique;
}