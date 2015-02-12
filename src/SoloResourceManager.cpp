#include "SoloResourceManager.h"
#include "SoloEffect.h"

using namespace solo;


ptr<Effect> ResourceManager::createEffect(const std::string& vsSrc, const std::string& fsSrc)
{
	return EffectFactory::create(vsSrc, fsSrc);
}


ptr<ResourceManager> ResourceManagerFactory::create()
{
	return NEW2(ResourceManager);
}