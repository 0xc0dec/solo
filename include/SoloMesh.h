#pragma once

#include "SoloBase.h"
#include "SoloVector3.h"
#include "SoloVector2.h"

namespace solo
{
	enum class EngineMode;

	class Mesh
	{
	public:
		static shared<Mesh> create(EngineMode mode);

		virtual ~Mesh() {}

		virtual void draw() = 0;

		virtual void setVertices(const std::vector<Vector3>& vertices) = 0;
		virtual void setNormals(const std::vector<Vector3>& normals) = 0;
		virtual void setUVs(const std::vector<Vector2>& uvs) = 0;
		virtual void setIndices(const std::vector<unsigned short>& indices) = 0;

	protected:
		Mesh() {}
		Mesh(const Mesh& other) = delete;
		Mesh(Mesh&& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;
		Mesh& operator=(Mesh&& other) = delete;
	};
}