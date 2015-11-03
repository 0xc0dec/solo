#include "SoloStubResourceManager.h"
#include "SoloStubImageLoader.h"
#include "SoloStubModelLoader.h"
#include "SoloDevice.h"

using namespace solo;


StubResourceManager::StubResourceManager(Device *device):
	ResourceManager(device)
{
	imageLoaders.clear();
	imageLoaders.push_back(SL_NEW<StubImageLoader>(device->getFileSystem(), this));

	modelLoaders.clear();
	modelLoaders.push_back(SL_NEW<StubModelLoader>(device->getFileSystem(), this));
}