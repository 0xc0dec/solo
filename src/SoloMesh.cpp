#include "SoloMesh.h"

using namespace solo;


std::shared_ptr<Mesh> Mesh::create()
{
	return NEW2(Mesh);
}