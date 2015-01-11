#include "SoloOpenGLMesh.h"

using namespace solo;


OpenGLMesh::OpenGLMesh():
	_vao(0),
	_vb(0)
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	GLfloat vertices[6][2] =
	{
		{ -0.95f, -0.95f },
		{ 0.95f, 0.95f },
		{ 0.95f, -0.95f },
		{ -0.95f, -0.95f },
		{ -0.95f, 0.95f },
		{ 0.95f, 0.95f }
	};
	glGenBuffers(1, &_vb);
	glBindBuffer(GL_ARRAY_BUFFER, _vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
}


ptr<OpenGLMesh> OpenGLMesh::create()
{
	return NEW2(OpenGLMesh);
}


void OpenGLMesh::draw()
{
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}