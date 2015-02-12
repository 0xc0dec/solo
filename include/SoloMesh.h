#pragma once

#include "SoloBase.h"
#include "SoloVector3.h"
#include "SoloVector2.h"

namespace solo
{
	class Mesh
	{
	public:
		virtual ~Mesh() {}

		virtual void draw() = 0;

	protected:
		Mesh(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs);
		Mesh(const Mesh& other) = delete;
		Mesh(Mesh&& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;
		Mesh& operator=(Mesh&& other) = delete;

		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Vector2> uvs;
	};

	class MeshFactory
	{
		friend class ResourceManager;
		static ptr<Mesh> create(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs);
	};
}