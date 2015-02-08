#include "SoloMesh.h"
#include "SoloException.h"
#include "platform/SoloOpenGLMesh.h"

using namespace solo;


ptr<Mesh> Mesh::create(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs)
{
	return OpenGLMesh::create(vertices, normals, uvs);
}


ptr<Mesh> Mesh::create(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals)
{
	return OpenGLMesh::create(vertices, normals);
}


ptr<Mesh> Mesh::create(const std::vector<Vector3>& vertices)
{
	return OpenGLMesh::create(vertices);
}


Mesh::Mesh(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs) :
	_vertices(vertices),
	_normals(normals),
	_uvs(uvs)
{
	auto verticesCount = vertices.size();
	auto normalsCount = normals.size();
	if (normalsCount > 0 && normalsCount != verticesCount)
		THROW(EngineException, "Number of normals (", normalsCount, ") doesn't equal to the number of vertices (", verticesCount, ")");
	auto uvsCount = uvs.size();
	if (uvsCount > 0 && uvsCount != verticesCount)
		THROW(EngineException, "Number of UVs (", uvsCount, ") doesn't equal to the number of vertices (", verticesCount, ")");
}