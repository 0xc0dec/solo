#include "SoloMesh.h"
#include "platform/SoloOpenGLMesh.h"

using namespace solo;


ptr<Mesh> Mesh::create()
{
	return OpenGLMesh::create();
}