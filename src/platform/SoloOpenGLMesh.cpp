#include "SoloOpenGLMesh.h"

using namespace solo;


ptr<OpenGLMesh> OpenGLMesh::create()
{
	return NEW2(OpenGLMesh);
}


void OpenGLMesh::draw()
{
}