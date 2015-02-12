#pragma once

#include "SoloBase.h"
#include "SoloVector2.h"
#include "SoloVector3.h"

namespace solo
{
	class Effect;
	class Material;
	class Mesh;
	class Model;

	class ResourceManager
	{
	public:
		~ResourceManager() {}

		Effect* createEffect(const std::string &vsSrc, const std::string &fsSrc);
		Material* createMaterial();
		Mesh* createMesh();
		Model* createModel();

	private:
		friend class ResourceManagerFactory;

		ResourceManager() {}
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager(ResourceManager&& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;
		ResourceManager& operator=(ResourceManager&& other) = delete;

		std::list<ptr<Effect>> effects;
		std::list<ptr<Material>> materials;
		std::list<ptr<Mesh>> meshes;
		std::list<ptr<Model>> models;
	};

	class ResourceManagerFactory
	{
		friend class Engine;
		static ptr<ResourceManager> create();
	};
}