#include "SoloStubResourceManager.h"
#include "SoloStubImageLoader.h"
#include "SoloStubModelLoader.h"
#include "SoloEngine.h"

using namespace solo;


StubResourceManager::StubResourceManager(Engine *engine):
	ResourceManager(engine)
{
	imageLoaders.clear();
	imageLoaders.push_back(NEW<StubImageLoader>(engine->getFileSystem(), this));

	modelLoaders.clear();
	modelLoaders.push_back(NEW<StubModelLoader>(engine->getFileSystem(), this));
}