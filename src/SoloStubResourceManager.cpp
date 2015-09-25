#include "SoloStubResourceManager.h"
#include "SoloStubTextureLoader.h"
#include "SoloStubModelLoader.h"
#include "SoloEngine.h"

using namespace solo;


StubResourceManager::StubResourceManager(Engine *engine):
	ResourceManager(engine)
{
	textureLoaders.clear();
	textureLoaders.push_back(NEW<StubTextureLoader>(engine->getFileSystem(), this));

	modelLoaders.clear();
	modelLoaders.push_back(NEW<StubModelLoader>(engine->getFileSystem(), this));
}