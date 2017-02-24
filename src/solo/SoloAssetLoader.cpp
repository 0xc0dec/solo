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
#define LIBASYNC_STATIC
#include <async++.h>


namespace solo
{
    struct AssetLoaderImpl
    {
        Device *device = nullptr;

        std::vector<sptr<ImageLoader>> imageLoaders;
        std::vector<sptr<MeshLoader>> meshLoaders;

        std::vector<async::task<std::function<void()>>> tasks;

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


auto AssetLoader::loadRectTextureAsync(const std::string &path) -> sptr<AsyncHandle<RectTexture>>
{
    auto handle = std::make_shared<AsyncHandle<RectTexture>>();
    auto loader = impl->getImageLoader(path);

    impl->tasks.push_back(async::spawn([=]
    {
        auto image = loader->load(path);
        return std::function<void(void)>([=]
        {
            auto texture = RectTexture::create(impl->device);
            // TODO this actually blocks and causes main thread/FPS stall. So we've offloaded
            // only part of the blocking task into another thread. Other similar places
            // have the same problem
            texture->setData(image->format, image->data.data(), image->width, image->height);
            handle->finish(texture);
        });
    }));

    return handle;
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


auto AssetLoader::loadCubeTextureAsync(const std::vector<std::string> &sidePaths) -> sptr<AsyncHandle<CubeTexture>>
{
    auto handle = std::make_shared<AsyncHandle<CubeTexture>>();
    auto loader = impl->getImageLoader(sidePaths[0]);

    std::vector<async::task<sptr<Image>>> imageTasks;
    for (uint32_t i = 0; i < sidePaths.size(); i++)
    {
        auto path = sidePaths[i];
        imageTasks.push_back(async::spawn([=] { return loader->load(path); }));
    }

    auto all = async::when_all(imageTasks.begin(), imageTasks.end())
        .then([=] (std::vector<async::task<sptr<Image>>> imageTasks)
        {
            std::vector<sptr<Image>> images;
            std::transform(imageTasks.begin(), imageTasks.end(), std::back_inserter(images),
                [](async::task<sptr<Image>> &t) { return t.get(); });

            return std::function<void(void)>(std::bind([=] (const std::vector<sptr<Image>> &images)
            {
                auto texture = CubeTexture::create(impl->device);
                uint32_t idx = 0;
                for (const auto &image : images)
                {
                    auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + idx++);
                    texture->setData(face, image->format, image->data.data(), image->width, image->height);
                }
                handle->finish(texture);
            }, std::move(images)));
        });

    impl->tasks.push_back(std::move(all));

    return handle;
}


auto AssetLoader::loadMesh(const std::string &path) -> sptr<Mesh>
{
    auto loader = impl->getMeshLoader(path);
    auto data = loader->loadData(path);
    return Mesh::createFromData(impl->device, data.get());
}


auto AssetLoader::loadMeshAsync(const std::string &path) -> sptr<AsyncHandle<Mesh>>
{
    auto handle = std::make_shared<AsyncHandle<Mesh>>();
    auto loader = impl->getMeshLoader(path);

    impl->tasks.push_back(async::spawn([=]
    {
        auto data = loader->loadData(path);
        return std::function<void(void)>([=]
        {
            auto mesh = Mesh::createFromData(impl->device, data.get());
            handle->finish(mesh);
        });
    }));

    return handle;
}


void AssetLoader::update()
{
    while (true)
    {
        auto removed = false;

        for (auto it = impl->tasks.begin(); it != impl->tasks.end(); ++it)
        {
            if (it->ready())
            {
                auto result = it->get();
                impl->tasks.erase(it);
                result();
                removed = true;
                break;
            }
        }
        
        if (!removed)
            break;
    }
}
