#include "SoloStubTextureLoader.h"
#include "SoloResourceManager.h"

using namespace solo;


StubTextureLoader::StubTextureLoader(FileSystem* fs, ResourceManager* resourceManager):
	TextureLoader(fs, resourceManager)
{
}


bool StubTextureLoader::isLoadable(const std::string& uri)
{
	return true;
}


shared<Texture2D> StubTextureLoader::load2D(const std::string& uri)
{
	return resourceManager->getOrCreateTexture2D(uri);
}