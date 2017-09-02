/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloAssetLoader.h"
#include "SoloMesh.h"
#include "SoloPngImageLoader.h"
#include "SoloRectTexture.h"
#include "SoloCubeTexture.h"
#include "SoloDevice.h"
#include "SoloObjMeshLoader.h"
#include "SoloImage.h"
#include <functional>

namespace solo
{
    struct AssetLoaderImpl
    {
        Device *device = nullptr;

        std::vector<sptr<ImageLoader>> imageLoaders;
        std::vector<sptr<MeshLoader>> meshLoaders;

        explicit AssetLoaderImpl(Device *device):
            device(device)
        {
            imageLoaders.push_back(std::make_unique<PngImageLoader>(device));
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

        auto getImageLoader(const std::string &path) -> ImageLoader*
        {
            for (const auto &l : imageLoaders)
            {
                if (l->isLoadable(path))
                    return l.get();
            }
            SL_PANIC(SL_FMT("No suitable loader found for image '", path, "'"));
            return nullptr;
        }
    };
}

using namespace solo;

AssetLoader::AssetLoader(Device *device, const FriendToken<Device> &)
{
    impl = std::make_unique<AssetLoaderImpl>(device);
}

auto AssetLoader::loadRectTexture(const std::string &path) -> sptr<RectTexture>
{
    auto loader = impl->getImageLoader(path);
    auto image = loader->load(path);
    auto result = RectTexture::create(impl->device);
    result->setData(image->format, image->data.data(), image->width, image->height);
    return result;
}

auto AssetLoader::loadCubeTexture(const std::vector<std::string> &sidePaths) -> sptr<CubeTexture>
{
    auto result = CubeTexture::create(impl->device);
    auto loader = impl->getImageLoader(sidePaths[0]);

    auto idx = 0;
    for (const auto &path : sidePaths)
    {
        auto image = loader->load(path);
        auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + idx++);
        result->setData(face, image->format, image->data.data(), image->width, image->height);
    }

    return result;
}

auto AssetLoader::loadMesh(const std::string &path) -> sptr<Mesh>
{
    auto loader = impl->getMeshLoader(path);
    auto data = loader->loadData(path);
    return Mesh::createFromData(impl->device, data.get());
}

