#include "SoloMaterialTechnique.h"

using namespace solo;


ptr<MaterialTechnique> MaterialTechnique::create()
{
	return NEW2(MaterialTechnique);
}


size_t MaterialTechnique::getPassCount() const
{
	return static_cast<unsigned>(_passes.size());
}


ptr<MaterialPass> MaterialTechnique::getPass(unsigned index) const
{
	return _passes[index];
}