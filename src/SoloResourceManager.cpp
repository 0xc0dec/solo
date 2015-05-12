#include "SoloResourceManager.h"
#include "SoloEffect.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"
#include "SoloModel.h"
#include "SoloPNGTextureLoader.h"
#include "SoloTexture2D.h"

using namespace solo;


shared<ResourceManager> ResourceManagerFactory::create()
{
	return NEW2(ResourceManager);
}


ResourceManager::ResourceManager()
{
	textureLoaders.push_back(NEW<PNGTextureLoader>());
}


std::string ResourceManager::calculateAutoUrl()
{
	auto url = std::string("Generated_") + std::to_string(++resourceCounter);
	return url;
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


shared<Effect> ResourceManager::findEffect(const std::string& url)
{
	auto existing = effects.find(url);
	return existing != effects.end() ? existing->second : nullptr;
}


shared<Material> ResourceManager::findMaterial(const std::string& url)
{
	auto existing = materials.find(url);
	return existing != materials.end() ? existing->second : nullptr;
}


shared<Texture> ResourceManager::findTexture(const std::string& url)
{
	auto existing = textures.find(url);
	return existing != textures.end() ? existing->second : nullptr;
}


shared<Mesh> ResourceManager::findMesh(const std::string& url)
{
	auto existing = meshes.find(url);
	return existing != meshes.end() ? existing->second : nullptr;
}


shared<Model> ResourceManager::findModel(const std::string& url)
{
	auto existing = models.find(url);
	return existing != models.end() ? existing->second : nullptr;
}


shared<Effect> ResourceManager::getOrCreateEffect(const std::string& vsSrc, const std::string& fsSrc)
{
	auto url = std::to_string(getHash(vsSrc + fsSrc));
	auto existing = findEffect(url);
	if (existing)
		return existing;
	auto effect = EffectFactory::create(vsSrc, fsSrc);
	effects[url] = effect;
	return effect;
}


shared<Material> ResourceManager::getOrCreateMaterial(shared<Effect> effect)
{
	auto effectUrl = findEffectUrl(effect);
	if (effectUrl.empty())
		THROW_FMT(EngineException, "Unknown effect");
	auto url = std::string("Material_") + effectUrl;
	auto existing = findMaterial(url);
	if (existing)
		return existing;
	auto material = MaterialFactory::create(effect);
	materials[url] = material;
	return material;
}


shared<Texture> ResourceManager::getOrCreateTexture(const std::string& url)
{
	auto existing = findTexture(url);
	if (existing)
		return existing;
	for (auto loader : textureLoaders)
	{
		if (loader->isLoadable(url))
		{
			auto texture = loader->load2D(url);
			textures[url] = texture;
			return texture;
		}
	}
	THROW_FMT(EngineException, "No suitable loader found for texture ", url);
}


shared<Mesh> ResourceManager::getOrCreateMesh(const std::string& url)
{
	auto existing = findMesh(url);
	if (existing)
		return existing;
	auto mesh = MeshFactory::create();
	meshes[url] = mesh;
	return mesh;
}


shared<Mesh> ResourceManager::getOrCreateMesh()
{
	auto url = calculateAutoUrl();
	return getOrCreateMesh(url);
}


shared<Model> ResourceManager::getOrCreateModel(const std::string& url)
{
	auto existing = findModel(url);
	if (existing)
		return existing;
	auto model = ModelFactory::create();
	models[url] = model;
	return model;
}


shared<Model> ResourceManager::getOrCreateModel()
{
	auto url = calculateAutoUrl();
	return getOrCreateModel(url);
}


template <typename T>
static void cleanUnusedResources(std::map<std::string, shared<T>> &resources)
{
	auto urls = std::unordered_set<std::string>();
	for (auto &it : resources)
	{
		if (it.second.use_count() == 1)
			urls.insert(it.first);
	}
	for (auto &url : urls)
		resources.erase(url);
}


void ResourceManager::cleanUnusedResources()
{
	// Clean in order of reference hierarchy. I.e. models reference materials,
	// materials reference effects, etc.
	::cleanUnusedResources(models);
	::cleanUnusedResources(materials);
	::cleanUnusedResources(effects);
	::cleanUnusedResources(meshes);
	::cleanUnusedResources(textures);
}