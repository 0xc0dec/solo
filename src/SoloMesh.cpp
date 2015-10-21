#include "SoloStubMesh.h"
#include "SoloEngineCreationArgs.h"
#include "platform/SoloOpenGLMesh.h"

using namespace solo;



shared<Mesh> MeshFactory::create(EngineMode mode)
{
	if (mode == EngineMode::OpenGL)
		return SL_NEW2(OpenGLMesh);
	return SL_NEW2(StubMesh);
}