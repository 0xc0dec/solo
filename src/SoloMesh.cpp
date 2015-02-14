#include "SoloMesh.h"
#include "platform/SoloOpenGLMesh.h"

using namespace solo;


shared<Mesh> MeshFactory::create()
{
	return NEW2(OpenGLMesh);
}
