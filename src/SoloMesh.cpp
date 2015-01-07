#include "SoloMesh.h"
#include "platform/SoloOpenGLMesh.h"

using namespace solo;


std::shared_ptr<Mesh> Mesh::create()
{
	return NEW<OpenGLMesh>();
}