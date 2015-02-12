#include "SoloMesh.h"
#include "platform/SoloOpenGLMesh.h"

using namespace solo;


ptr<Mesh> MeshFactory::create()
{
	return NEW2(OpenGLMesh);
}
