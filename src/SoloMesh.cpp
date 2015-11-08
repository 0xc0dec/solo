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


void Mesh::rebuildAsQuad()
{
	setVertices({
		Vector3(-1, -1, 0),
		Vector3(-1, 1, 0),
		Vector3(1, 1, 0),
		Vector3(1, -1, 0)
	});
	setNormals({
		Vector3(0, 0, -1),
		Vector3(0, 0, -1),
		Vector3(0, 0, -1),
		Vector3(0, 0, -1)
	});
	setUVs({
		Vector2(0, 0),
		Vector2(0, 1),
		Vector2(1, 1),
		Vector2(1, 0)
	});
	setIndices({
		0, 1, 2,
		0, 2, 3
	});
}
