#include "SoloMaterial.h"

using namespace solo;


ptr<Material> Material::create()
{
	return ALLOC_WITH_DELETER(Material);
}


unsigned Material::getTechniquesCount() const
{
	return static_cast<unsigned>(_techniques.size());
}


ptr<Technique> Material::getTechnique(unsigned index) const
{
	// No bounds checks intentionally. It's user's responsibility to a pass correct index.
	return _techniques[index];
}