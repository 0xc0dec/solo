#pragma once

#include "SoloBase.h"

namespace solo
{
	class Effect;
	class Material;
	class Mesh;
	class Model;
	class Texture2D;
	class RenderTarget;
	class TextureLoader;
	class ModelLoader;
	class Engine;

	class ResourceManager
	{
	public:
		~ResourceManager() {}

		shared<Effect> findEffect(const std::string &uri);
		shared<Texture2D> findTexture(const std::string &uri);
		shared<Material> findMaterial(const std::string &uri);
		shared<Mesh> findMesh(const std::string &uri);
		shared<Model> findModel(const std::string &uri);
		shared<RenderTarget> findRenderTarget(const std::string &uri);

		shared<Effect> getOrCreateEffect(const std::string &vsSrc, const std::string &fsSrc, const std::string& uri = "");
		shared<Texture2D> getOrCreateTexture2D(const std::string &uri = "");
		shared<Material> getOrCreateMaterial(shared<Effect> effect, const std::string& uri = "");
		shared<Mesh> getOrCreateMesh(const std::string& uri = "");
		shared<Model> getOrCreateModel(const std::string& uri = "");
		shared<RenderTarget> getOrCreateRenderTarget(const std::string &uri);

		shared<Texture2D> getOrLoadTexture(const std::string &uri);
		shared<Model> getOrLoadModel(const std::string& uri);

		void cleanUnusedResources();

	private:
		friend class ResourceManagerFactory;

		template <typename TResource> using ResourceMap = std::unordered_map<std::string, shared<TResource>>;

		explicit ResourceManager(Engine *engine);
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager(ResourceManager&& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;
		ResourceManager& operator=(ResourceManager&& other) = delete;

		std::string generateUri();
		
		template <typename TResource>
		static void cleanUnusedResources(ResourceMap<TResource> &resources);

		template <typename TResource>
		shared<TResource> getOrCreateResource(const std::string& uri, ResourceMap<TResource>& resourceMap,
			std::function<shared<TResource>(const std::basic_string<char>&)> find,
			std::function<shared<TResource>()> create);

		template <typename TResource>
		shared<TResource> findResource(const std::string& uri, const ResourceMap<TResource>& resourceMap);

		Engine *engine{ nullptr };

		ResourceMap<Effect> effects;
		ResourceMap<Material> materials;
		ResourceMap<Mesh> meshes;
		ResourceMap<Model> models;
		ResourceMap<Texture2D> textures;
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