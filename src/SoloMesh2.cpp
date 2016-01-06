#include "SoloMesh2.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLMesh2.h"

using namespace solo;


shared<Mesh2> Mesh2::create(DeviceMode mode, const VertexFormat &vertexFormat, bool dynamic)
{
//	if (mode == DeviceMode::OpenGL)
//		return
	// TODO stub mesh2
	return SL_NEW_SHARED(OpenGLMesh2, vertexFormat, dynamic);
}


Mesh2::Mesh2(const VertexFormat& vertexFormat, bool dynamic):
	vertexFormat(vertexFormat),
	dynamic(dynamic)
{
}
