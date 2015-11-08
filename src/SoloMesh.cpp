#include "SoloDevice.h"
#include "platform/stub/SoloStubMesh.h"
#include "platform/opengl/SoloOpenGLMesh.h"

using namespace solo;


shared<Mesh> Mesh::create(DeviceMode mode)
{
	if (mode == DeviceMode::OpenGL)
		return SL_NEW2(OpenGLMesh);
	return SL_NEW2(StubMesh);
}
