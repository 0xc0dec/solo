#include "SoloOpenGLMesh.h"

using namespace solo;


OpenGLMesh::OpenGLMesh():
	Mesh()
{
	glGenVertexArrays(1, &vertexArrayHandle);
	if (!vertexArrayHandle)
		THROW_FMT(EngineException, "Failed to obtain mesh handle");
}


GLuint OpenGLMesh::buildElementArrayBuffer(GLuint existingHandle, const std::vector<unsigned short>& elements)
{
	auto handle = existingHandle ? existingHandle : 0;
	if (!handle)
		glGenBuffers(1, &handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned short), elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return handle;
}


template<typename TElement>
GLuint OpenGLMesh::buildArrayBuffer(GLuint existingHandle, const std::vector<TElement>& elements, GLuint elementSize, GLuint vertexAttributeIndex, GLenum elementType)
{
	auto handle = existingHandle ? existingHandle : 0;
	if (!handle)
		glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(TElement), elements.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexAttributeIndex);
	glVertexAttribPointer(vertexAttributeIndex, elementSize, elementType, GL_FALSE, 0, nullptr);
	return handle;
}


OpenGLMesh::~OpenGLMesh()
{
	if (vertexBufferHandle)
		glDeleteBuffers(1, &vertexBufferHandle);
	if (normalBufferHandle)
		glDeleteBuffers(1, &normalBufferHandle);
	if (uvBufferHandle)
		glDeleteBuffers(1, &uvBufferHandle);
	if (indicesBufferHandle)
		glDeleteBuffers(1, &indicesBufferHandle);
	glDeleteVertexArrays(1, &vertexArrayHandle);
}


void OpenGLMesh::draw()
{
	glBindVertexArray(vertexArrayHandle);
	if (indicesCount > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferHandle);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesCount), GL_UNSIGNED_SHORT, nullptr);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(verticesCount));
}


void OpenGLMesh::setVertices(const std::vector<Vector3>& vertices)
{
	if (!vertices.empty())
	{
		glBindVertexArray(vertexArrayHandle);
		vertexBufferHandle = buildArrayBuffer(vertexBufferHandle, vertices, 3, 0, GL_FLOAT);
		verticesCount = vertices.size();
	}
}


void OpenGLMesh::setNormals(const std::vector<Vector3>& normals)
{
	if (!normals.empty())
	{
		glBindVertexArray(vertexArrayHandle);
		normalBufferHandle = buildArrayBuffer(normalBufferHandle, normals, 3, 1, GL_FLOAT);
		normalsCount = normals.size();
	}
}


void OpenGLMesh::setUVs(const std::vector<Vector2>& uvs)
{
	if (!uvs.empty())
	{
		glBindVertexArray(vertexArrayHandle);
		uvBufferHandle = buildArrayBuffer(uvBufferHandle, uvs, 2, 2, GL_FLOAT);
		uvsCount = uvs.size();
	}
}


void OpenGLMesh::setIndices(const std::vector<unsigned short>& indices)
{
	if (!indices.empty())
	{
		glBindVertexArray(vertexArrayHandle);
		indicesBufferHandle = buildElementArrayBuffer(indicesBufferHandle, indices);
		indicesCount = indices.size();
	}
}