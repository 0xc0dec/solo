/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloAssetLoader.h"
#include "SoloMesh.h"
#include "SoloDevice.h"
#include "SoloObjMeshLoader.h"
#include <functional>

namespace solo
{
    struct AssetLoaderImpl
    {
        Device *device = nullptr;

        std::vector<sptr<MeshLoader>> meshLoaders;

        explicit AssetLoaderImpl(Device *device):
            device(device)
        {
            meshLoaders.push_back(std::make_unique<ObjMeshLoader>(device));
        }

        auto getMeshLoader(const std::string &path) -> MeshLoader*
        {
            for (const auto &loader : meshLoaders)
            {
                if (loader->isLoadable(path))
                    return loader.get();
            }
            SL_PANIC(SL_FMT("No suitable loader found for mesh '", path, "'"));
            return nullptr;
        }
    };
}

using namespace solo;

AssetLoader::AssetLoader(Device *device, const FriendToken<Device> &)
{
    impl = std::make_unique<AssetLoaderImpl>(device);
}

auto AssetLoader::loadMesh(const std::string &path) -> sptr<Mesh>
{
    auto loader = impl->getMeshLoader(path);
    auto data = loader->loadData(path);
    return Mesh::createFromData(impl->device, data.get());
}

