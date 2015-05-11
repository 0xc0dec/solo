#pragma once

#include "SoloBase.h"

namespace solo
{
	class Effect;
	class Material;
	class Mesh;
	class Model;
	class Texture;
	class TextureLoader;

	class ResourceManager
	{
	public:
		~ResourceManager() {}

		shared<Effect> findEffect(const std::string &url);
		shared<Material> findMaterial(const std::string &url);
		shared<Texture> findTexture(const std::string &url);
		shared<Mesh> findMesh(const std::string &url);
		shared<Model> findModel(const std::string &url);

		shared<Effect> getOrCreateEffect(const std::string &vsSrc, const std::string &fsSrc);
		shared<Material> getOrCreateMaterial(shared<Effect> effect);
		shared<Texture> getOrCreateTexture(const std::string& url);
		shared<Mesh> getOrCreateMesh(const std::string& url);
		shared<Mesh> getOrCreateMesh();
		shared<Model> getOrCreateModel(const std::string& url);
		shared<Model> getOrCreateModel();

		void cleanUnusedResources();

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
		std::map<std::string, shared<Texture>> textures;

		std::vector<shared<TextureLoader>> textureLoaders;

		size_t resourceCounter { 0 };
	};

	class ResourceManagerFactory
	{
		friend class Engine;
		static shared<ResourceManager> create();
	};
}