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
		shared<Material> getMaterial(const std::string& id);
		shared<Material> getMaterial();
		shared<Mesh> getMesh(const std::string& id);
		shared<Mesh> getMesh();
		shared<Model> getModel(const std::string& id);
		shared<Model> getModel();

	private:
		friend class ResourceManagerFactory;

		ResourceManager();
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager(ResourceManager&& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;
		ResourceManager& operator=(ResourceManager&& other) = delete;

		std::string getAutoGeneratedId();

		std::map<std::string, shared<Effect>> effects;
		std::map<std::string, shared<Material>> materials;
		std::map<std::string, shared<Mesh>> meshes;
		std::map<std::string, shared<Model>> models;

		size_t idCounter;
	};

	class ResourceManagerFactory
	{
		friend class Engine;
		static shared<ResourceManager> create();
	};
}