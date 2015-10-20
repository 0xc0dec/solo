#include <functional>
#include "SoloResourceManager.h"
#include "SoloStubResourceManager.h"
#include "SoloEffect.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"
#include "SoloModel.h"
#include "SoloRenderTarget.h"
#include "SoloPngImageLoader.h"
#include "SoloTexture2D.h"
#include "SoloTextureCube.h"
#include "SoloEngine.h"
#include "SoloObjModelLoader.h"

using namespace solo;


shared<ResourceManager> ResourceManagerFactory::create(Engine *engine)
{
	if (engine->getMode() == EngineMode::Stub)
		return NEW2(StubResourceManager, engine);
	return NEW2(ResourceManager, engine);
}


ResourceManager::ResourceManager(Engine *engine):
	engine(engine)
{
	imageLoaders.push_back(NEW<PngImageLoader>(engine->getFileSystem(), this));
	modelLoaders.push_back(NEW<ObjModelLoader>(engine->getFileSystem(), this));
}


std::string ResourceManager::generateUri()
{
	return std::string("generated/") + std::to_string(++resourceCounter);
}


shared<Effect> ResourceManager::findEffect(const std::string& uri)
{
	return findResource(uri, effects);
}


shared<Material> ResourceManager::findMaterial(const std::string& uri)
{
	return findResource(uri, materials);
}


shared<Texture2D> ResourceManager::findTexture2D(const std::string& uri)
{
	return findResource(uri, textures2d);
}


shared<TextureCube> ResourceManager::findTextureCube(const std::string& uri)
{
	return findResource(uri, texturesCube);
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


shared<Effect> ResourceManager::getOrCreateEffect(const std::string& vsSrc, const std::string& fsSrc, const std::string& uri)
{
	return getOrCreateResource<Effect>(uri, effects,
		std::bind(&ResourceManager::findEffect, this, std::placeholders::_1),
		[&]() { return EffectFactory::create(engine->getMode(), vsSrc, fsSrc); });
}


shared<Material> ResourceManager::getOrCreateMaterial(shared<Effect> effect, const std::string& uri)
{
	// effectively ignores the effect if a material with the given uri already exists
	return getOrCreateResource<Material>(uri, materials,
		std::bind(&ResourceManager::findMaterial, this, std::placeholders::_1),
		[&]() { return MaterialFactory::create(engine->getMode(), effect); });
}


shared<Texture2D> ResourceManager::getOrLoadTexture2D(const std::string& uri)
{
	auto existing = findTexture2D(uri);
	if (existing)
		return existing;
	for (auto loader : imageLoaders)
	{
		if (loader->isLoadable(uri))
		{
			auto result = TextureFactory::create2D(engine->getMode());
			auto image = loader->load(uri);
			result->setData(image->colorFormat, image->data, image->width, image->height);
			textures2d[uri] = result;
			return result;
		}
	}
	THROW_FMT(EngineException, "No suitable loader found for texture ", uri);
}


shared<TextureCube> ResourceManager::getOrLoadTextureCube(const std::vector<std::string>& imageUris)
{
	if (imageUris.size() != 6)
		THROW_FMT(EngineException, "Wrong number of faces for cube texture (", imageUris.size(), " provided, 6 expected)");
	auto uri = imageUris[0] + imageUris[1] + imageUris[2] + imageUris[3] + imageUris[4] + imageUris[5];
	auto existing = findTextureCube(uri);
	if (existing)
		return existing;
	auto result = TextureFactory::createCube(engine->getMode());
	auto idx = 0;
	for (auto& imageUri: imageUris)
	{
		shared<Image> image;
		for (auto loader : imageLoaders)
		{
			if (loader->isLoadable(imageUri))
			{
				image = loader->load(imageUri);
				auto face = static_cast<int>(TextureCubeFace::Front) + idx;
				result->setData(static_cast<TextureCubeFace>(face), image->colorFormat, image->data, image->width, image->height);
				break;
			}
		}
		if (!image)
			THROW_FMT(EngineException, "No suitable loader found for texture ", imageUri);
		idx++;
	}
	texturesCube[uri] = result;
	return result;
}


shared<Texture2D> ResourceManager::getOrCreateTexture2D(const std::string &uri)
{
	return getOrCreateResource<Texture2D>(uri, textures2d,
		std::bind(&ResourceManager::findTexture2D, this, std::placeholders::_1),
		std::bind(&TextureFactory::create2D, engine->getMode()));
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
		std::bind(&ResourceManager::findMesh, this, std::placeholders::_1),
		std::bind(&MeshFactory::create, engine->getMode()));
}


shared<RenderTarget> ResourceManager::getOrCreateRenderTarget(const std::string& uri)
{
	return getOrCreateResource<RenderTarget>(uri, renderTargets,
		std::bind(&ResourceManager::findRenderTarget, this, std::placeholders::_1),
		std::bind(&RenderTargetFactory::create, engine->getMode()));
}


template <typename TResource>
shared<TResource> ResourceManager::getOrCreateResource(const std::string& uri, ResourceMap<TResource>& resourceMap,
	std::function<shared<TResource>(const std::basic_string<char>&)> find,
	std::function<shared<TResource>()> create)
{
	auto existing = find(uri.empty() ? generateUri() : uri);
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
	auto uris = std::unordered_set<std::string>();
	for (auto &it : resources)
	{
		if (it.second.use_count() == 1)
			uris.insert(it.first);
	}
	for (auto &uri : uris)
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
	cleanUnusedResources(textures2d);
	cleanUnusedResources(texturesCube);
}