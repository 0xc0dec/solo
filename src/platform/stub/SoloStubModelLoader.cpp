#include "SoloStubModelLoader.h"
#include "SoloResourceManager.h"

using namespace solo;


StubMeshLoader::StubMeshLoader(FileSystem *fs, ResourceManager *resourceManager)
    : MeshLoader(fs, resourceManager)
{
}


bool StubMeshLoader::isLoadable(const std::string &uri)
{
    return true;
}


shared<Mesh2> StubMeshLoader::load(const std::string &uri)
{
    return resourceManager->getOrCreateMesh(uri);
}