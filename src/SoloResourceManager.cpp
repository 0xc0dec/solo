#include <functional>
#include "SoloResourceManager.h"
#include "SoloEffect.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"
#include "SoloModel.h"
#include "SoloRenderTarget.h"
#include "SoloPngTextureLoader.h"
#include "SoloTexture2D.h"
#include "SoloEngine.h"
#include "SoloObjModelLoader.h"

using namespace solo;


shared<ResourceManager> ResourceManagerFactory::create(Engine *engine)
{
	return NEW2(ResourceManager, engine);
}


ResourceManager::ResourceManager(Engine *engine):
	engine(engine)
{
	textureLoaders.push_back(NEW<PngTextureLoader>(engine->getFileSystem()));
	modelLoaders.push_back(NEW<ObjModelLoader>(engine->getFileSystem()));
}


std::string ResourceManager::calculateAutoUrl()
{
	return std::string("Generated_") + std::to_string(++resourceCounter);
}


std::string ResourceManager::findEffectUrl(shared<Effect> effect) const
{
	for (auto pair : effects)
	{
		if (pair.second == effect)
			return pair.first;
	}
	return {};
}


shared<Effect> ResourceManager::findEffect(const std::string& uri)
{
	return findResource(uri, effects);
}


shared<Material> ResourceManager::findMaterial(const std::string& uri)
{
	return findResource(uri, materials);
}


shared<Texture> ResourceManager::findTexture(const std::string& uri)
{
	return findResource(uri, textures);
}


shared<Mesh> ResourceManager::findMesh(const std::string& uri)
{
	return findResource(uri, meshes);
}


shared<Model> ResourceManager::findModel(const std::string& uri)
{
	return findResource(uri, models);
}


shared<RenderTarget> ResourceManager::findRenderTarget(const std::string& uri)
{
	return findResource(uri, renderTargets);
}


shared<Effect> ResourceManager::getOrCreateEffect(const std::string& vsSrc, const std::string& fsSrc)
{
	auto uri = std::to_string(getHash(vsSrc + fsSrc));
	return getOrCreateResource<Effect>(uri, effects,
		std::bind(&ResourceManager::findEffect, this, std::placeholders::_1),
		[&]() { return EffectFactory::create(vsSrc, fsSrc); });
}


shared<Material> ResourceManager::createMaterial(shared<Effect> effect)
{
	auto effectUrl = findEffectUrl(effect);
	if (effectUrl.empty())
		THROW_FMT(EngineException, "Unknown effect");
	auto uri = calculateAutoUrl();
	auto material = MaterialFactory::create(effect);
	materials[uri] = material;
	return material;
}


shared<Texture> ResourceManager::getOrLoadTexture(const std::string& uri)
{
	auto existing = findTexture(uri);
	if (existing)
		return existing;
	for (auto loader : textureLoaders)
	{
		if (loader->isLoadable(uri))
		{
			auto texture = loader->load2D(uri);
			textures[uri] = texture;
			return texture;
		}
	}
	THROW_FMT(EngineException, "No suitable loader found for texture ", uri);
}


shared<Texture> ResourceManager::getOrCreateTexture(const std::string &uri)
{
	return getOrCreateResource<Texture>(uri, textures,
		std::bind(&ResourceManager::findTexture, this, std::placeholders::_1), &TextureFactory::create2D);
}


shared<Model> ResourceManager::getOrLoadModel(const std::string& uri)
{
	auto existing = findModel(uri);
	if (existing)
		return existing;
	for (auto loader : modelLoaders)
	{
		if (loader->isLoadable(uri))
		{
			auto model = loader->load(uri);
			models[uri] = model;
			return model;
		}
	}
	THROW_FMT(EngineException, "No suitable loader found for model ", uri);
}


shared<Model> ResourceManager::getOrCreateModel(const std::string& uri)
{
	return getOrCreateResource<Model>(uri, models,
		std::bind(&ResourceManager::findModel, this, std::placeholders::_1), &ModelFactory::create);
}


shared<Mesh> ResourceManager::getOrCreateMesh(const std::string& uri)
{
	return getOrCreateResource<Mesh>(uri, meshes,
		std::bind(&ResourceManager::findMesh, this, std::placeholders::_1), &MeshFactory::create);
}


shared<RenderTarget> ResourceManager::getOrCreateRenderTarget(const std::string& uri)
{
	return getOrCreateResource<RenderTarget>(uri, renderTargets,
		std::bind(&ResourceManager::findRenderTarget, this, std::placeholders::_1), &RenderTargetFactory::create);
}


template <typename TResource>
shared<TResource> ResourceManager::getOrCreateResource(const std::string& uri, ResourceMap<TResource>& resourceMap,
	std::function<shared<TResource>(const std::basic_string<char>&)> find,
	std::function<shared<TResource>()> create)
{
	auto existing = find(uri.empty() ? calculateAutoUrl() : uri);
	if (existing)
		return existing;
	auto result = create();
	resourceMap[uri] = result;
	return result;
}


template <typename TResource>
shared<TResource> ResourceManager::findResource(const std::string& uri, const ResourceMap<TResource>& resourceMap)
{
	auto existing = resourceMap.find(uri);
	return existing != resourceMap.end() ? existing->second : nullptr;
}


template <typename TResource>
void ResourceManager::cleanUnusedResources(ResourceMap<TResource> &resources)
{
	auto urls = std::unordered_set<std::string>();
	for (auto &it : resources)
	{
		if (it.second.use_count() == 1)
			urls.insert(it.first);
	}
	for (auto &uri : urls)
		resources.erase(uri);
}


void ResourceManager::cleanUnusedResources()
{
	// Clean in order of reference hierarchy. I.e. models reference materials,
	// materials reference effects, etc.
	cleanUnusedResources(models);
	cleanUnusedResources(materials);
	cleanUnusedResources(effects);
	cleanUnusedResources(meshes);
	cleanUnusedResources(textures);
}