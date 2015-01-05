#include "SoloMaterial.h"

using namespace solo;


ptr<Material> Material::create()
{
	return ALLOC_WITH_DELETER(Material);
}