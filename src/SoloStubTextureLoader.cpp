#include "SoloStubTextureLoader.h"
#include "SoloResourceManager.h"
#include "SoloImage.h"

using namespace solo;


StubTextureLoader::StubTextureLoader(FileSystem* fs, ResourceManager* resourceManager):
	TextureLoader(fs, resourceManager)
{
}


bool StubTextureLoader::isLoadable(const std::string& uri)
{
	return true;
}


shared<Image> StubTextureLoader::load2D(const std::string& uri)
{
	return NEW2(Image);
}