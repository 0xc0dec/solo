/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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

using namespace solo;


AssetLoader::AssetLoader(Device *device, const DeviceToken &)
{
    imageLoaders.push_back(std::make_unique<PngImageLoader>(device));
    meshLoaders.push_back(std::make_unique<ObjMeshLoader>(device));
}


auto AssetLoader::getMeshLoader(const std::string &path) -> MeshLoader *
{
    for (const auto &loader : meshLoaders)
    {
        if (loader->isLoadable(path))
            return loader.get();
    }
    SL_ERR(SL_FMT("No suitable loader found for mesh '", path, "'"));
    return nullptr;
}


auto AssetLoader::getImageLoader(const std::string &path) -> ImageLoader *
{
    for (const auto &l : imageLoaders)
    {
        if (l->isLoadable(path))
            return l.get();
    }
    SL_ERR(SL_FMT("No suitable loader found for image '", path, "'"));
    return nullptr;
}


auto AssetLoader::loadRectTexture(const std::string &path) -> sptr<RectTexture>
{
    auto loader = getImageLoader(path);
    auto image = loader->load(path);
    auto result = RectTexture::create();
    result->setData(image->format, image->data.data(), image->width, image->height);
    return result;
}


auto AssetLoader::loadRectTextureAsync(const std::string &path) -> sptr<AsyncHandle<RectTexture>>
{
    auto handle = std::make_shared<AsyncHandle<RectTexture>>();
    auto loader = getImageLoader(path);
    async::spawn([ = ]
    {
        auto image = loader->load(path);
        volatile auto lock = this->tasksLock.acquire();
        this->tasks.push_back([ = ]()
        {
            auto texture = RectTexture::create();
            // TODO this actually blocks and causes main thread/FPS stall. So we've offloaded
            // only part of the blocking task into another thread. Other similar places
            // have the same problem
            texture->setData(image->format, image->data.data(), image->width, image->height);
            handle->finish(texture);
        });
    });

    return handle;
}


auto AssetLoader::loadCubeTexture(const std::vector<std::string> &sidePaths) -> sptr<CubeTexture>
{
    auto result = CubeTexture::create();
    auto loader = getImageLoader(sidePaths[0]);

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
    auto loader = getImageLoader(sidePaths[0]);

    std::vector<async::task<sptr<Image>>> imageTasks;
    for (uint32_t i = 0; i < sidePaths.size(); i++)
    {
        auto path = sidePaths[i];
        imageTasks.push_back(async::spawn([ = ]
        {
            return loader->load(path);
        }));
    }

    async::when_all(imageTasks.begin(), imageTasks.end()).then([ = ] (std::vector<async::task<sptr<Image>>> imageTasks)
    {
        std::vector<sptr<Image>> images;
        std::transform(imageTasks.begin(), imageTasks.end(), std::back_inserter(images),
                       [](async::task<sptr<Image>> &t)
        {
            return t.get();
        });

        volatile auto lock = this->tasksLock.acquire();
        this->tasks.push_back(std::bind([ = ] (const std::vector<sptr<Image>> &images)
        {
            auto texture = CubeTexture::create();
            uint32_t idx = 0;
            for (const auto &image : images)
            {
                auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + idx++);
                texture->setData(face, image->format, image->data.data(), image->width, image->height);
            }
            handle->finish(texture);
        }, std::move(images)));
    });

    return handle;
}


auto AssetLoader::loadMesh(const std::string &path) -> sptr<Mesh>
{
    auto loader = getMeshLoader(path);
    auto data = loader->loadData(path);
    return Mesh::create(data.get());
}


auto AssetLoader::loadMeshAsync(const std::string &path) -> sptr<AsyncHandle<Mesh>>
{
    auto handle = std::make_shared<AsyncHandle<Mesh>>();
    auto loader = getMeshLoader(path);

    async::spawn([ = ]
    {
        auto data = loader->loadData(path);
        volatile auto lock = this->tasksLock.acquire();
        this->tasks.push_back([ = ]()
        {
            // This is later called in the update() method
            auto mesh = Mesh::create(data.get());
            handle->finish(mesh);
        });
    });

    return handle;
}


void AssetLoader::update()
{
    if (!tasks.empty())
    {
        volatile auto lt = tasksLock.acquire();
        if (!tasks.empty())
        {
            auto func = std::move(tasks.back());
            func();
            tasks.pop_back();
        }
    }
}
