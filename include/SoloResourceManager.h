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

		shared<Effect> getEffect(const std::string &vsSrc, const std::string &fsSrc);
		Material* getMaterial();
		Mesh* getMesh();
		Model* getModel();

	private:
		friend class ResourceManagerFactory;

		ResourceManager() {}
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager(ResourceManager&& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;
		ResourceManager& operator=(ResourceManager&& other) = delete;

		std::list<shared<Effect>> effects;
		std::list<shared<Material>> materials;
		std::list<shared<Mesh>> meshes;
		std::list<shared<Model>> models;
	};

	class ResourceManagerFactory
	{
		friend class Engine;
		static shared<ResourceManager> create();
	};
}