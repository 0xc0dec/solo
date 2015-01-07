#include "SoloMaterial.h"
#include "SoloException.h"

using namespace solo;


ptr<Material> Material::create()
{
	return NEW2(Material);
}


void Material::addTechnique(ptr<MaterialTechnique> technique)
{
	_techniques.push_back(technique);
	if (!_currentTechnique)
		_currentTechnique = technique;
}


void Material::removeTechnique(ptr<MaterialTechnique> technique)
{
	auto where = find(_techniques.begin(), _techniques.end(), technique);
	if (where != _techniques.end())
		_techniques.erase(where);
	if (_currentTechnique == technique)
		_currentTechnique = nullptr;
}


void Material::setCurrentTechnique(ptr<MaterialTechnique> technique)
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


ptr<MaterialTechnique> Material::getTechnique(unsigned index) const
{
	return _techniques[index];
}


ptr<MaterialTechnique> Material::getCurrentTechnique() const
{
	return _currentTechnique;
}


ptr<MaterialParameter> Material::findParameter(const std::string& name)
{
	auto where = _parameters.find(name);
	return where != _parameters.end() ? where->second : nullptr;
}