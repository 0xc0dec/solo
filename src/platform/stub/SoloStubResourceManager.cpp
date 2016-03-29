//#include "SoloStubResourceManager.h"
//#include "SoloStubImageLoader.h"
//#include "SoloStubMeshLoader.h"
//#include "SoloDevice.h"
//
//using namespace solo;
//
//
//StubResourceManager::StubResourceManager(Device* device):
//    ResourceManager(device)
//{
//    imageLoaders.clear();
//    imageLoaders.push_back(std::make_unique<StubImageLoader>(device->getFileSystem(), this));
//
//    meshLoaders.clear();
//    meshLoaders.push_back(std::make_unique<StubMeshLoader>(device->getFileSystem(), this));
//}