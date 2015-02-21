#include "SoloResourceManager.h"
#include "SoloEffect.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"
#include "SoloModel.h"

using namespace solo;


shared<ResourceManager> ResourceManagerFactory::create()
{
	return NEW2(ResourceManager);
}


ResourceManager::ResourceManager():
	resourceCounter(0)
{
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
	return std::string();
}


shared<Effect> ResourceManager::getEffect(const std::string& vsSrc, const std::string& fsSrc)
{
	auto url = std::to_string(getHash(vsSrc + fsSrc));
	auto existing = effects.find(url);
	if (existing != effects.end())
		return existing->second;
	auto effect = EffectFactory::create(vsSrc, fsSrc);
	effects[url] = effect;
	return effect;
}


shared<Material> ResourceManager::getMaterial(shared<Effect> effect)
{
	auto effectUrl = findEffectUrl(effect);
	if (effectUrl.empty())
		THROW(EngineException, "Unknown effect");
	auto url = std::string("Material_") + effectUrl;
	auto existing = materials.find(url);
	if (existing != materials.end())
		return existing->second;
	auto material = MaterialFactory::create(effect);
	materials[url] = material;
	return material;
}


shared<Mesh> ResourceManager::getMesh(const std::string& url)
{
	auto existing = meshes.find(url);
	if (existing != meshes.end())
		return existing->second;
	auto mesh = MeshFactory::create();
	meshes[url] = mesh;
	return mesh;
}


shared<Mesh> ResourceManager::getMesh()
{
	auto url = calculateAutoUrl();
	return getMesh(url);
}


shared<Model> ResourceManager::getModel(const std::string& url)
{
	auto existing = models.find(url);
	if (existing != models.end())
		return existing->second;
	auto model = ModelFactory::create();
	models[url] = model;
	return model;
}


shared<Model> ResourceManager::getModel()
{
	auto url = calculateAutoUrl();
	return getModel(url);
}