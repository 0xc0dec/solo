#include "SoloStubImageLoader.h"
#include "SoloResourceManager.h"
#include "SoloImage.h"

using namespace solo;


StubImageLoader::StubImageLoader(FileSystem* fs, ResourceManager* resourceManager):
    ImageLoader(fs, resourceManager)
{
}


bool StubImageLoader::isLoadable(const std::string& uri)
{
    return true;
}


unique<Image> StubImageLoader::load(const std::string& uri)
{
    return SL_MAKE_UNIQUE<Image>();
}