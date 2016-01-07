#include "SoloStubResourceManager.h"
#include "SoloStubImageLoader.h"
#include "SoloStubModelLoader.h"
#include "SoloDevice.h"

using namespace solo;


StubResourceManager::StubResourceManager(Device *device):
    ResourceManager(device)
{
    imageLoaders.clear();
    imageLoaders.push_back(SL_MAKE_SHARED<StubImageLoader>(device->getFileSystem(), this));

    meshLoaders.clear();
    meshLoaders.push_back(SL_MAKE_SHARED<StubMeshLoader>(device->getFileSystem(), this));
}