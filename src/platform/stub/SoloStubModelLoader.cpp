#include "SoloStubModelLoader.h"
#include "SoloResourceManager.h"

using namespace solo;


StubModelLoader::StubModelLoader(FileSystem *fs, ResourceManager *resourceManager)
    : ModelLoader(fs, resourceManager)
{
}


bool StubModelLoader::isLoadable(const std::string &uri)
{
    return true;
}


shared<Model> StubModelLoader::load(const std::string &uri)
{
    return resourceManager->getOrCreateModel(uri);
}