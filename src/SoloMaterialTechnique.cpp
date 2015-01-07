#include "SoloMaterialTechnique.h"

using namespace solo;


std::shared_ptr<MaterialTechnique> MaterialTechnique::create()
{
	return NEW2(MaterialTechnique);
}


size_t MaterialTechnique::getPassCount() const
{
	return static_cast<unsigned>(_passes.size());
}


std::shared_ptr<Pass> MaterialTechnique::getPass(unsigned index) const
{
	return _passes[index];
}