#include "SoloOpenGLMesh2.h"

using namespace solo;


OpenGLMesh2::OpenGLMesh2(const VertexFormat& vertexFormat, bool dynamic):
	Mesh2(vertexFormat, dynamic)
{
	glGenBuffers(1, &vertexBufferHandle);
	if (!vertexBufferHandle)
		SL_THROW_FMT(EngineException, "Unable to obtain mesh handle");
}


void OpenGLMesh2::setVertexData(float *data, unsigned vertexCount, unsigned firstVertexIndex)
{
	if (vertexCount == 0 || firstVertexIndex >= vertexCount)
		SL_THROW_FMT(EngineException, "Invalid combination of first vertex index and vertex count parameters (have ", firstVertexIndex, " and ", vertexCount, ")");

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);

	if (firstVertexIndex + vertexCount > lastVertexCount)
		glBufferData(GL_ARRAY_BUFFER, vertexFormat.getVertexSize() * vertexCount, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	else
		glBufferSubData(GL_ARRAY_BUFFER, firstVertexIndex * vertexFormat.getVertexSize(), vertexCount * vertexFormat.getVertexSize(), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
