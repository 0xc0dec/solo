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
	if (_currentTechnique == technique)
		_currentTechnique = nullptr;
}


void Material::setCurrentTechnique(ptr<Technique> technique)
{
	auto where = find(_techniques.begin(), _techniques.end(), technique);
	if (where == _techniques.end())
		THROW(EngineException, "Material doesn't contain the specified technique");
	_currentTechnique = technique;
}


void Material::setCurrentTechnique(unsigned index)
{
	_currentTechnique = _techniques[index];
}


size_t Material::getTechniquesCount() const
{
	return _techniques.size();
}


ptr<Technique> Material::getTechnique(unsigned index) const
{
	return _techniques[index];
}


ptr<Technique> Material::getCurrentTechnique() const
{
	return _currentTechnique;
}