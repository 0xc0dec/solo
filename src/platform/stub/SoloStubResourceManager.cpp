#include "SoloStubResourceManager.h"
#include "SoloStubImageLoader.h"
#include "SoloStubMeshLoader.h"
#include "SoloDevice.h"

using namespace solo;


StubResourceManager::StubResourceManager(Device* device):
    ResourceManager(device, 0)
{
    imageLoaders.clear();
    imageLoaders.push_back(SL_MAKE_UNIQUE<StubImageLoader>(device->getFileSystem(), this));

    meshLoaders.clear();
    meshLoaders.push_back(SL_MAKE_UNIQUE<StubMeshLoader>(device->getFileSystem(), this));
}