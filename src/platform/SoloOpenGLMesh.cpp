#include "SoloOpenGLMesh.h"

using namespace solo;


OpenGLMesh::OpenGLMesh(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs) :
	Mesh(vertices, normals, uvs),
	_vertexArrayHandle(0),
	_vertexBufferHandle(0),
	_normalBufferHandle(0),
	_uvBufferHandle(0)
{
	glGenVertexArrays(1, &_vertexArrayHandle);
	glBindVertexArray(_vertexArrayHandle);

	glGenBuffers(1, &_vertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), _vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (!normals.empty())
	{
		glGenBuffers(1, &_normalBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, _normalBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(Vector3), _normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	if (!uvs.empty())
	{
		glGenBuffers(1, &_uvBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(Vector2), _uvs.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);
}


OpenGLMesh::~OpenGLMesh()
{
	glDeleteBuffers(1, &_vertexBufferHandle);
	if (_normalBufferHandle)
		glDeleteBuffers(1, &_normalBufferHandle);
	if (_uvBufferHandle)
		glDeleteBuffers(1, &_uvBufferHandle);
	glDeleteVertexArrays(1, &_vertexArrayHandle);
}


ptr<OpenGLMesh> OpenGLMesh::create(const std::vector<Vector3>& vertices)
{
	return NEW2(OpenGLMesh, vertices, {}, {});
}


ptr<OpenGLMesh> OpenGLMesh::create(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals)
{
	return NEW2(OpenGLMesh, vertices, normals, {});
}


ptr<OpenGLMesh> OpenGLMesh::create(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs)
{
	return NEW2(OpenGLMesh, vertices, normals, uvs);
}


void OpenGLMesh::draw()
{
	glBindVertexArray(_vertexArrayHandle);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_vertices.size()));
	glBindVertexArray(0);
}