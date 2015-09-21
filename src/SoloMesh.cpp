#include "SoloStubMesh.h"
#include "SoloEngineCreationArgs.h"
#include "platform/SoloOpenGLMesh.h"

using namespace solo;


shared<Mesh> Mesh::create(EngineMode mode)
{
	if (mode == EngineMode::OpenGL)
		return NEW2(OpenGLMesh);
	return NEW2(StubMesh);
}