#include "SoloResourceManager.h"
#include "SoloEffect.h"
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


ResourceManager::ResourceManager(const DeviceToken&)
{
    imageLoaders.push_back(std::make_unique<PngImageLoader>());
    meshLoaders.push_back(std::make_unique<ObjMeshLoader>());
}


ResourceManager::~ResourceManager()
{
}


MeshLoader* ResourceManager::getMeshLoader(const std::string& uri)
{
    for (const auto& loader : meshLoaders)
    {
        if (loader->isLoadable(uri))
            return loader.get();
    }
    SL_FMT_THROW(ResourceException, "No suitable loader found for mesh ", uri);
}


ImageLoader* ResourceManager::getImageLoader(const std::string& uri)
{
    for (const auto& l : imageLoaders)
    {
        if (l->isLoadable(uri))
            return l.get();
    }
    SL_FMT_THROW(ResourceException, "No suitable loader found for image ", uri);
}


sptr<Texture2D> ResourceManager::loadTexture2D(const std::string& imageUri)
{
    for (const auto& loader : imageLoaders)
    {
        if (loader->isLoadable(imageUri))
        {
            auto result = Texture2D::create();
            auto image = loader->load(imageUri);
            result->setData(image->colorFormat, image->data, image->width, image->height);
            return result;
        }
    }

    SL_FMT_THROW(ResourceException, "No suitable loader found for image ", imageUri);
}


sptr<AsyncResourceHandle<Texture2D>> ResourceManager::loadTexture2DAsync(const std::string& imageUri)
{
    auto handle = std::make_shared<AsyncResourceHandle<Texture2D>>();
    auto loader = getImageLoader(imageUri);
    async::spawn([=]
    {
        auto uimage = loader->load(imageUri);
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


sptr<CubeTexture> ResourceManager::loadCubeTexture(const std::vector<std::string>& sidesUris)
{
    auto result = CubeTexture::create();
    auto loader = getImageLoader(sidesUris[0]);

    auto idx = 0;
    for (const auto& imageUri : sidesUris)
    {
        auto image = loader->load(imageUri);
        auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + idx++);
        result->setData(face, image->colorFormat, image->data, image->width, image->height);
    }

    return result;
}


sptr<AsyncResourceHandle<CubeTexture>> ResourceManager::loadCubeTextureAsync(const std::vector<std::string>& sidesUris)
{
    auto handle = std::make_shared<AsyncResourceHandle<CubeTexture>>();
    auto loader = getImageLoader(sidesUris[0]);
    auto device = Device::get();

    std::vector<async::task<sptr<Image>>> imageTasks;
    for (uint32_t i = 0; i < sidesUris.size(); i++)
    {
        auto sizeUri = sidesUris[i];
        imageTasks.push_back(async::spawn([=]
        {
            return sptr<Image>{ loader->load(sizeUri) }; // making it shared simplifies tasks management
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


sptr<Mesh> ResourceManager::loadMesh(const std::string& dataUri)
{
    auto loader = getMeshLoader(dataUri);
    auto data = loader->loadData(dataUri);
    return Mesh::create(data.get());
}


sptr<AsyncResourceHandle<Mesh>> ResourceManager::loadMeshAsync(const std::string& dataUri)
{
    auto handle = std::make_shared<AsyncResourceHandle<Mesh>>();
    auto loader = getMeshLoader(dataUri);

    async::spawn([=]
    {
        auto data = loader->loadData(dataUri);
        sptr<MeshData> sharedData{ std::move(data) };
        auto lock = this->tasksLock.acquire();
        this->tasks.push_back([=]()
        {
            // This called is later called in the update() method
            auto mesh = Mesh::create(sharedData.get());
            handle->putResult(mesh);
        });
    });

    return handle;
}


void ResourceManager::update()
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