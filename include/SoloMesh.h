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

		static ptr<Mesh> create(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs);
		static ptr<Mesh> create(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals);
		static ptr<Mesh> create(const std::vector<Vector3>& vertices);

		virtual void draw() = 0;

	protected:
		Mesh(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& uvs);
		Mesh(const Mesh& other) = default;
		Mesh(Mesh&& other);
		Mesh& operator=(const Mesh& other) = default;
		Mesh& operator=(Mesh&& other);

		std::vector<Vector3> _vertices;
		std::vector<Vector3> _normals;
		std::vector<Vector2> _uvs;
	};
}