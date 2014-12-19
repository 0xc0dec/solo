#include "SoloTransform.h"

using namespace solo;


size_t Transform::getTypeId()
{
	static auto hash = computeHash("Transform");
	return hash;
}