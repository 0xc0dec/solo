#pragma once

#include <GL/glew.h>
#include "SoloBase.h"
#include "SoloMesh.h"
#include "SoloVector3.h"

namespace solo
{
	class OpenGLMesh : public Mesh
	{
	public:
		virtual ~OpenGLMesh() override;

		virtual void draw() override;

	private:
		friend class MeshFactory;

		OpenGLMesh(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs);
		OpenGLMesh(const OpenGLMesh& other) = delete;
		OpenGLMesh(OpenGLMesh&& other) = delete;
		OpenGLMesh& operator=(const OpenGLMesh& other) = delete;
		OpenGLMesh& operator=(OpenGLMesh&& other) = delete;

		GLuint vertexArrayHandle;
		GLuint vertexBufferHandle;
		GLuint normalBufferHandle;
		GLuint uvBufferHandle;
	};
}