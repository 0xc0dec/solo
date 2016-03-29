//#include "SoloStubMeshLoader.h"
//#include "SoloResourceManager.h"
//#include "SoloMesh.h"
//
//using namespace solo;
//
//
//StubMeshLoader::StubMeshLoader(FileSystem* fs, ResourceManager* resourceManager)
//    : MeshLoader(fs, resourceManager)
//{
//}
//
//
//bool StubMeshLoader::isLoadable(const std::string& uri)
//{
//    return true;
//}
//
//
//sptr<Mesh> StubMeshLoader::load(const std::string& uri)
//{
//    return resourceManager->getOrCreateMesh(uri);
//}
//
//
//uptr<MeshData> StubMeshLoader::loadData(const std::string& uri)
//{
//    return std::make_unique<MeshData>();
//}
