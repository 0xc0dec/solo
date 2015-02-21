#pragma once

#include "SoloBase.h"

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
		shared<Material> getMaterial(shared<Effect> effect);
		shared<Mesh> getMesh(const std::string& url);
		shared<Mesh> getMesh();
		shared<Model> getModel(const std::string& url);
		shared<Model> getModel();

	private:
		friend class ResourceManagerFactory;

		ResourceManager();
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager(ResourceManager&& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;
		ResourceManager& operator=(ResourceManager&& other) = delete;

		std::string calculateAutoUrl();
		std::string findEffectUrl(shared<Effect> effect) const;

		std::map<std::string, shared<Effect>> effects;
		std::map<std::string, shared<Material>> materials;
		std::map<std::string, shared<Mesh>> meshes;
		std::map<std::string, shared<Model>> models;

		size_t resourceCounter;
	};

	class ResourceManagerFactory
	{
		friend class Engine;
		static shared<ResourceManager> create();
	};
}