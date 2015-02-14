#include "SoloResourceManager.h"
#include "SoloEffect.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"
#include "SoloModel.h"

using namespace solo;


shared <ResourceManager> ResourceManagerFactory::create()
{
	return NEW2(ResourceManager);
}


Effect* ResourceManager::getEffect(const std::string &vsSrc, const std::string &fsSrc)
{
	auto effect = EffectFactory::create(vsSrc, fsSrc);
	effects.push_back(effect);
	return effect.get();
}


Material* ResourceManager::getMaterial()
{
	auto material = MaterialFactory::create();
	materials.push_back(material);
	return material.get();
}


Mesh* ResourceManager::createMesh()
{
	auto mesh = MeshFactory::create();
	meshes.push_back(mesh);
	return mesh.get();
}


Model* ResourceManager::createModel()
{
	auto model = ModelFactory::create();
	models.push_back(model);
	return model.get();
}