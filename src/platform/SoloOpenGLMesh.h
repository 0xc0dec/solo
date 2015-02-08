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

		static ptr<OpenGLMesh> create(const std::vector<Vector3>& vertices);
		static ptr<OpenGLMesh> create(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals);
		static ptr<OpenGLMesh> create(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs);

		virtual void draw() override;

	private:
		OpenGLMesh(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs);
		OpenGLMesh(const OpenGLMesh& other) = default;
		OpenGLMesh(OpenGLMesh&& other);
		OpenGLMesh& operator=(const OpenGLMesh& other) = default;
		OpenGLMesh& operator=(OpenGLMesh&& other);

		GLuint _vertexArrayHandle;
		GLuint _vertexBufferHandle;
		GLuint _normalBufferHandle;
		GLuint _uvBufferHandle;
	};
}