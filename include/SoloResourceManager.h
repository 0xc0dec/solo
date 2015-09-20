#pragma once

#include "SoloBase.h"

namespace solo
{
	class Effect;
	class Material;
	class Mesh;
	class Model;
	class Texture;
	class RenderTarget;
	class TextureLoader;
	class ModelLoader;
	class Engine;

	class ResourceManager
	{
	public:
		~ResourceManager() {}

		shared<Effect> findEffect(const std::string &url);
		shared<Material> findMaterial(const std::string &url);
		shared<Texture> findTexture(const std::string &url);
		shared<Mesh> findMesh(const std::string &url);
		shared<Model> findModel(const std::string &url);
		shared<RenderTarget> findRenderTarget(const std::string &url);

		shared<Material> createMaterial(shared<Effect> effect);
		shared<Effect> getOrCreateEffect(const std::string &vsSrc, const std::string &fsSrc);
		shared<Texture> getOrLoadTexture(const std::string &url);
		shared<Texture> getOrCreateTexture(const std::string &url = "");
		shared<Model> getOrCreateModel(const std::string& url = "");
		shared<Model> getOrLoadModel(const std::string& url);
		shared<Mesh> getOrCreateMesh(const std::string& url = "");
		shared<RenderTarget> getOrCreateRenderTarget(const std::string &url);

		void cleanUnusedResources();

	private:
		friend class ResourceManagerFactory;

		template <typename TResource> using ResourceMap = std::unordered_map<std::string, shared<TResource>>;

		ResourceManager(Engine *engine);
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager(ResourceManager&& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;
		ResourceManager& operator=(ResourceManager&& other) = delete;

		std::string calculateAutoUrl();
		std::string findEffectUrl(shared<Effect> effect) const;
		
		template <typename TResource>
		static void cleanUnusedResources(ResourceMap<TResource> &resources);

		Engine *engine{ nullptr };

		ResourceMap<Effect> effects;
		ResourceMap<Material> materials;
		ResourceMap<Mesh> meshes;
		ResourceMap<Model> models;
		ResourceMap<Texture> textures;
		ResourceMap<RenderTarget> renderTargets;

		std::vector<shared<TextureLoader>> textureLoaders;
		std::vector<shared<ModelLoader>> modelLoaders;

		size_t resourceCounter{ 0 };
	};

	class ResourceManagerFactory
	{
		friend class Engine;
		static shared<ResourceManager> create(Engine *engine);
	};
}