#include "SoloOpenGLMesh.h"

using namespace solo;


OpenGLMesh::OpenGLMesh(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs) :
	Mesh(vertices, normals, uvs),
	vertexArrayHandle(0),
	vertexBufferHandle(0),
	normalBufferHandle(0),
	uvBufferHandle(0)
{
	glGenVertexArrays(1, &vertexArrayHandle);
	glBindVertexArray(vertexArrayHandle);

	glGenBuffers(1, &vertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (!normals.empty())
	{
		glGenBuffers(1, &normalBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3), normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	if (!uvs.empty())
	{
		glGenBuffers(1, &uvBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vector2), uvs.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);
}


OpenGLMesh::~OpenGLMesh()
{
	glDeleteBuffers(1, &vertexBufferHandle);
	if (normalBufferHandle)
		glDeleteBuffers(1, &normalBufferHandle);
	if (uvBufferHandle)
		glDeleteBuffers(1, &uvBufferHandle);
	glDeleteVertexArrays(1, &vertexArrayHandle);
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
	glBindVertexArray(vertexArrayHandle);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
	glBindVertexArray(0);
}