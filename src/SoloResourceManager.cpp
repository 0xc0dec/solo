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


shared<RenderTarget> ResourceManager::findRenderTarget(const std::string& url)
{
	auto existing = renderTargets.find(url);
	return existing != renderTargets.end() ? existing->second : nullptr;
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


shared<Material> ResourceManager::createMaterial(shared<Effect> effect)
{
	auto effectUrl = findEffectUrl(effect);
	if (effectUrl.empty())
		THROW_FMT(EngineException, "Unknown effect.");
	auto url = calculateAutoUrl();
	auto material = MaterialFactory::create(effect);
	materials[url] = material;
	return material;
}


shared<Texture> ResourceManager::getOrLoadTexture(const std::string& url)
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


shared<Texture> ResourceManager::getOrCreateTexture(const std::string &url)
{
	auto existing = findTexture(url.empty() ? calculateAutoUrl() : url);
	if (existing)
		return existing;
	auto texture = TextureFactory::create2D();
	textures[url] = texture;
	return texture;
}


shared<Model> ResourceManager::getOrLoadModel(const std::string& url)
{
	auto existing = findModel(url);
	if (existing)
		return existing;
	for (auto loader : modelLoaders)
	{
		if (loader->isLoadable(url))
		{
			auto model = loader->load(url);
			models[url] = model;
			return model;
		}
	}
	THROW_FMT(EngineException, "No suitable loader found for model ", url);
}


shared<Model> ResourceManager::getOrCreateModel(const std::string& url)
{
	auto existing = findModel(url.empty() ? calculateAutoUrl() : url);
	if (existing)
		return existing;
	auto model = ModelFactory::create();
	models[url] = model;
	return model;
}


//template <typename TResource, typename TResourceRepo>
//shared<TResource> getOrCreateResource(const std::string& url, std::function<shared<TResource>(const std::string&)> find)
//{
//	auto existing = find(url.empty() ? calculateAutoUrl() : url);
//	if (existing)
//		return existing;
//	auto mesh = MeshFactory::create();
//	meshes[url] = mesh;
//	return mesh;
//}


shared<Mesh> ResourceManager::getOrCreateMesh(const std::string& url)
{
	auto existing = findMesh(url.empty() ? calculateAutoUrl() : url);
	if (existing)
		return existing;
	auto mesh = MeshFactory::create();
	meshes[url] = mesh;
	return mesh;
}


shared<RenderTarget> ResourceManager::getOrCreateRenderTarget(const std::string& url)
{
	auto existing = findRenderTarget(url.empty() ? calculateAutoUrl() : url);
	if (existing)
		return existing;
	auto target = RenderTargetFactory::create();
	renderTargets[url] = target;
	return target;
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
	for (auto &url : urls)
		resources.erase(url);
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