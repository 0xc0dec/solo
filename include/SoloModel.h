#pragma once

#include "SoloBase.h"

namespace solo
{
	class Mesh;

	// Represents a renderable 3D model composed of one or more meshes.
	class Model
	{
	public:
		~Model() {}

		static ptr<Model> create();

		void addMesh(const ptr<Mesh> mesh);
		void removeMesh(const ptr<Mesh> mesh);
		ptr<Mesh> getMesh(unsigned index) const;
		size_t getMeshCount() const;

	private:
		Model() {}
		std::vector<ptr<Mesh>> _meshes;
	};
}