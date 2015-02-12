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

		virtual void setVertices(const std::vector<Vector3>& vertices) override;
		virtual void setNormals(const std::vector<Vector3>& normals) override;
		virtual void setUVs(const std::vector<Vector2>& uvs) override;
		virtual void setIndices(const std::vector<unsigned short>& indices) override;

	private:
		friend class MeshFactory;

		OpenGLMesh();
		OpenGLMesh(const OpenGLMesh& other) = delete;
		OpenGLMesh(OpenGLMesh&& other) = delete;
		OpenGLMesh& operator=(const OpenGLMesh& other) = delete;
		OpenGLMesh& operator=(OpenGLMesh&& other) = delete;

		template<typename TElement>
		static GLuint initArrayBufferBuffer(const std::vector<TElement>& elements, GLuint elementSize, GLuint index, GLenum elementType);
		static GLuint initElementArrayBufferBuffer(const std::vector<unsigned short>& elements);

		GLuint vertexArrayHandle;
		GLuint vertexBufferHandle;
		GLuint normalBufferHandle;
		GLuint uvBufferHandle;
		GLuint indicesBufferHandle;

		size_t verticesCount;
		size_t normalsCount;
		size_t uvsCount;
		size_t indicesCount;
	};
}