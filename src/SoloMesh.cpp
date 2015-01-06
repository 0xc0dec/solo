#include "SoloMesh.h"


std::shared_ptr<solo::Mesh> solo::Mesh::create()
{
	return ALLOC_WITH_DELETER(Mesh);
}