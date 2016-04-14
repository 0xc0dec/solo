#include "SoloAssetLoader.h"
#include "SoloMesh.h"
#include "SoloPngImageLoader.h"
#include "SoloTexture2D.h"
#include "SoloCubeTexture.h"
#include "SoloDevice.h"
#include "SoloObjMeshLoader.h"
#include "SoloImage.h"
#include <functional>
#define LIBASYNC_STATIC
#include <async++.h>

using namespace solo;


AssetLoader::AssetLoader(const DeviceToken&)
{
    imageLoaders.push_back(std::make_unique<PngImageLoader>());
    meshLoaders.push_back(std::make_unique<ObjMeshLoader>());
}


AssetLoader::~AssetLoader()
{
}


auto AssetLoader::getMeshLoader(const std::string& path) -> MeshLoader*
{
    for (const auto& loader : meshLoaders)
    {
        if (loader->isLoadable(path))
            return loader.get();
    }
    SL_FMT_THROW(AssetException, "No suitable loader found for mesh ", path);
}


auto AssetLoader::getImageLoader(const std::string& path) -> ImageLoader*
{
    for (const auto& l : imageLoaders)
    {
        if (l->isLoadable(path))
            return l.get();
    }
    SL_FMT_THROW(AssetException, "No suitable loader found for image ", path);
}


auto AssetLoader::loadTexture2D(const std::string& path) -> sptr<Texture2D>
{
    auto loader = getImageLoader(path);
    auto image = loader->load(path);
    auto result = Texture2D::create();
    result->setData(image->colorFormat, image->data, image->width, image->height);
    return result;
}


auto AssetLoader::loadTexture2DAsync(const std::string& path) -> sptr<AsyncHandle<Texture2D>>
{
    auto handle = std::make_shared<AsyncHandle<Texture2D>>();
    auto loader = getImageLoader(path);
    async::spawn([=]
    {
        auto uimage = loader->load(path);
        sptr<Image> simage { std::move(uimage) };
        auto lock = this->tasksLock.acquire();
        this->tasks.push_back([=]()
        {
            auto texture = Texture2D::create();
            texture->setData(simage->colorFormat, simage->data, simage->width, simage->height);
            handle->putResult(texture);
        });
    });

    return handle;
}


auto AssetLoader::loadCubeTexture(const std::vector<std::string>& sidesPaths) -> sptr<CubeTexture>
{
    auto result = CubeTexture::create();
    auto loader = getImageLoader(sidesPaths[0]);

    auto idx = 0;
    for (const auto& path : sidesPaths)
    {
        auto image = loader->load(path);
        auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + idx++);
        result->setData(face, image->colorFormat, image->data, image->width, image->height);
    }

    return result;
}


auto AssetLoader::loadCubeTextureAsync(const std::vector<std::string>& sidesPaths) -> sptr<AsyncHandle<CubeTexture>>
{
    auto handle = std::make_shared<AsyncHandle<CubeTexture>>();
    auto loader = getImageLoader(sidesPaths[0]);
    auto device = Device::get();

    std::vector<async::task<sptr<Image>>> imageTasks;
    for (uint32_t i = 0; i < sidesPaths.size(); i++)
    {
        auto path = sidesPaths[i];
        imageTasks.push_back(async::spawn([=]
        {
            return sptr<Image>{ loader->load(path) }; // making it shared simplifies tasks management
        }));
    }

    async::when_all(imageTasks.begin(), imageTasks.end()).then([=] (std::vector<async::task<sptr<Image>>> imageTasks)
    {
        std::vector<sptr<Image>> images;
        std::transform(imageTasks.begin(), imageTasks.end(), std::back_inserter(images), [](async::task<sptr<Image>>& t) { return t.get(); });

        auto lock = this->tasksLock.acquire();
        this->tasks.push_back(std::bind([=] (const std::vector<sptr<Image>>& images)
        {
            auto texture = CubeTexture::create();
            uint32_t idx = 0;
            for (const auto& image : images)
            {
                auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + idx++);
                texture->setData(face, image->colorFormat, image->data, image->width, image->height);
            }
            handle->putResult(texture);
        }, std::move(images)));
    });

    return handle;
}


auto AssetLoader::loadMesh(const std::string& path) -> sptr<Mesh>
{
    auto loader = getMeshLoader(path);
    auto data = loader->loadData(path);
    return Mesh::create(data.get());
}


auto AssetLoader::loadMeshAsync(const std::string& path) -> sptr<AsyncHandle<Mesh>>
{
    auto handle = std::make_shared<AsyncHandle<Mesh>>();
    auto loader = getMeshLoader(path);

    async::spawn([=]
    {
        auto data = loader->loadData(path);
        sptr<MeshData> sharedData{ std::move(data) };
        auto lock = this->tasksLock.acquire();
        this->tasks.push_back([=]()
        {
            // This is later called in the update() method
            auto mesh = Mesh::create(sharedData.get());
            handle->putResult(mesh);
        });
    });

    return handle;
}


void AssetLoader::update()
{
    if (!tasks.empty())
    {
        auto lt = tasksLock.acquire();
        if (!tasks.empty())
        {
            auto func = std::move(tasks.back());
            func();
            tasks.pop_back();
        }
    }
}