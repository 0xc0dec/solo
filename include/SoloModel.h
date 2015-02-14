#pragma once

#include "SoloBase.h"

namespace solo
{
	class Mesh;

	class Model
	{
	public:
		~Model() {}

		void addMesh(Mesh* mesh);
		void removeMesh(Mesh* mesh);
		Mesh* getMesh(unsigned index) const;
		size_t getMeshCount() const;

	private:
		friend class ModelFactory;

		Model() {}
		Model(const Model& other) = delete;
		Model(Model&& other) = delete;
		Model& operator=(const Model& other) = delete;
		Model& operator=(Model&& other) = delete;

		std::vector<Mesh*> meshes;
	};

	class ModelFactory
	{
		friend class ResourceManager;
		static shared<Model> create();
	};
}