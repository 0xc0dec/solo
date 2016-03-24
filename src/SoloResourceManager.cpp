#include "SoloResourceManager.h"
#include "SoloEffect.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"
#include "SoloFrameBuffer.h"
#include "SoloPngImageLoader.h"
#include "SoloTexture2D.h"
#include "SoloCubeTexture.h"
#include "SoloDevice.h"
#include "SoloObjMeshLoader.h"
#include "SoloImage.h"
#include "platform/stub/SoloStubResourceManager.h"
#include <functional>
#define LIBASYNC_STATIC
#include <async++.h>

using namespace solo;


sptr<ResourceManager> ResourceManager::create(Device* device)
{
    if (device->getMode() == DeviceMode::Stub)
        return SL_NEW_SHARED(StubResourceManager, device);
    return SL_NEW_SHARED(ResourceManager, device);
}


ResourceManager::ResourceManager(Device* device):
    device(device)
{
    imageLoaders.push_back(std::make_unique<PngImageLoader>(device->getFileSystem(), this));
    meshLoaders.push_back(std::make_unique<ObjMeshLoader>(device->getFileSystem(), this));
}


ResourceManager::~ResourceManager()
{
}


std::string ResourceManager::generateUri()
{
    return std::string("/generated/") + std::to_string(++resourceCounter);
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


sptr<Effect> ResourceManager::findEffect(const std::string& uri)
{
    return findResource(uri, effects);
}


sptr<Material> ResourceManager::findMaterial(const std::string& uri)
{
    return findResource(uri, materials);
}


sptr<Texture2D> ResourceManager::findTexture2D(const std::string& uri)
{
    return findResource(uri, textures2d);
}


sptr<CubeTexture> ResourceManager::findCubeTexture(const std::string& uri)
{
    return findResource(uri, cubeTextures);
}


sptr<Mesh> ResourceManager::findMesh(const std::string& uri)
{
    return findResource(uri, meshes);
}


sptr<FrameBuffer> ResourceManager::findFrameBuffer(const std::string& uri)
{
    return findResource(uri, frameBuffers);
}


sptr<Effect> ResourceManager::getOrCreateEffect(const std::string& vsSrc, const std::string& fsSrc, const std::string& uri)
{
    return getOrCreateResource<Effect>(uri, effects,
        std::bind(&ResourceManager::findEffect, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Effect, device->getRenderer(), vsSrc, fsSrc); });
}


sptr<Effect> ResourceManager::getOrCreatePrefabEffect(EffectPrefab prefab, const std::string& uri)
{
    return getOrCreateResource<Effect>(uri, effects,
        std::bind(&ResourceManager::findEffect, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Effect, device->getRenderer(), prefab); });
}


sptr<Material> ResourceManager::getOrCreateMaterial(sptr<Effect> effect, const std::string& uri)
{
    return getOrCreateResource<Material>(uri, materials,
        std::bind(&ResourceManager::findMaterial, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Material, device->getRenderer(), effect); });
}


sptr<Texture2D> ResourceManager::getOrCreateTexture2D(const std::string& uri)
{
    return getOrCreateResource<Texture2D>(uri, textures2d,
        std::bind(&ResourceManager::findTexture2D, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Texture2D, device->getRenderer()); });
}


sptr<CubeTexture> ResourceManager::getOrCreateCubeTexture(const std::string& uri)
{
    return getOrCreateResource<CubeTexture>(uri, cubeTextures,
        std::bind(&ResourceManager::findCubeTexture, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(CubeTexture, device->getRenderer()); });
}


sptr<Texture2D> ResourceManager::getOrLoadTexture2D(const std::string& imageUri, const std::string& uri)
{
    auto textureUri = uri.empty() ? imageUri : uri;
    auto existing = findTexture2D(textureUri);
    if (existing)
        return existing;

    for (const auto& loader : imageLoaders)
    {
        if (loader->isLoadable(imageUri))
        {
            auto result = SL_NEW_SHARED(Texture2D, device->getRenderer());
            auto image = loader->load(imageUri);
            result->setData(image->colorFormat, image->data, image->width, image->height);
            textures2d[textureUri] = result;
            return result;
        }
    }

    SL_FMT_THROW(ResourceException, "No suitable loader found for image ", imageUri);
}


sptr<CubeTexture> ResourceManager::getOrLoadCubeTexture(const std::vector<std::string>& sidesUris, const std::string& uri)
{
    auto textureUri = uri.empty()
        ? sidesUris[0] + sidesUris[1] + sidesUris[2] + sidesUris[3] + sidesUris[4] + sidesUris[5]
        : uri;
    auto existing = findCubeTexture(textureUri);
    if (existing)
        return existing;

    auto result = SL_NEW_SHARED(CubeTexture, device->getRenderer());
    auto loader = getImageLoader(sidesUris[0]);

    auto idx = 0;
    for (const auto& imageUri : sidesUris)
    {
        auto image = loader->load(imageUri);
        auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + idx++);
        result->setData(face, image->colorFormat, image->data, image->width, image->height);
    }

    cubeTextures[textureUri] = result;
    return result;
}


void ResourceManager::getOrLoadCubeTextureAsync(const std::vector<std::string>& sidesUris, std::function<void(sptr<CubeTexture>)> callback, const std::string& uri)
{
    auto textureUri = uri.empty()
        ? sidesUris[0] + sidesUris[1] + sidesUris[2] + sidesUris[3] + sidesUris[4] + sidesUris[5] // TODO avoid copypasting
        : uri;
    auto existing = findCubeTexture(textureUri);
    if (existing)
    {
        callback(existing);
        return;
    }

    auto loader = getImageLoader(sidesUris[0]);

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
            auto texture = std::make_shared<CubeTexture>(this->device->getRenderer());
            uint32_t idx = 0;
            for (const auto& image : images)
            {
                auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + idx++);
                texture->setData(face, image->colorFormat, image->data, image->width, image->height);
            }
            callback(texture);
        }, std::move(images)));
    });
}


sptr<Mesh> ResourceManager::getOrLoadMesh(const std::string& dataUri, const std::string& uri)
{
    auto meshUri = uri.empty() ? dataUri : uri;
    auto existing = findMesh(meshUri);
    if (existing)
        return existing;

    auto loader = getMeshLoader(dataUri);
    auto mesh = loader->load(dataUri);
    meshes[meshUri] = mesh;
    return mesh;
}


void ResourceManager::getOrLoadMeshAsync(const std::string& dataUri, std::function<void(sptr<Mesh>)> callback, const std::string& uri)
{
    auto meshUri = uri.empty() ? dataUri : uri;
    auto existing = findMesh(meshUri);
    if (existing)
    {
        callback(existing);
        return;
    }

    auto loader = getMeshLoader(dataUri);
    
    async::spawn([=] {
        auto data = loader->loadData(dataUri);
        sptr<MeshData> sharedData{ std::move(data) };
        auto lock = this->tasksLock.acquire();
        this->tasks.push_back([=]()
        {
            // This called is later called in the update() method
            auto mesh = std::make_shared<Mesh>(this->device->getRenderer(), sharedData.get());
            this->meshes[meshUri] = mesh;
            callback(mesh);
        });
    });
}


sptr<Mesh> ResourceManager::getOrCreateMesh(const std::string& uri)
{
    return getOrCreateResource<Mesh>(uri, meshes,
        std::bind(&ResourceManager::findMesh, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Mesh, device->getRenderer()); });
}


sptr<Mesh> ResourceManager::getOrCreatePrefabMesh(MeshPrefab prefab, const std::string& uri)
{
    return getOrCreateResource<Mesh>(uri, meshes,
        std::bind(&ResourceManager::findMesh, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Mesh, device->getRenderer(), prefab); });
}


sptr<FrameBuffer> ResourceManager::getOrCreateFrameBuffer(const std::string& uri)
{
    return getOrCreateResource<FrameBuffer>(uri, frameBuffers,
        std::bind(&ResourceManager::findFrameBuffer, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(FrameBuffer, device->getRenderer()); });
}


template <typename TResource>
sptr<TResource> ResourceManager::getOrCreateResource(
    const std::string& uri,
    ResourceCollection<TResource>& resourceMap,
    std::function<sptr<TResource>(const std::basic_string<char>&)> find,
    std::function<sptr<TResource>()> create)
{
    auto existing = find(uri.empty() ? generateUri() : uri); // TODO use findResource here?
    return existing ? existing : createResource<TResource>(uri, resourceMap, create);
}


template <typename TResource>
sptr<TResource> ResourceManager::createResource(const std::string& uri, ResourceCollection<TResource>& resourceMap, std::function<sptr<TResource>()> create)
{
    auto result = create();
    resourceMap[uri] = result;
    return result;
}


template <typename TResource>
sptr<TResource> ResourceManager::findResource(const std::string& uri, const ResourceCollection<TResource>& resourceMap)
{
    auto existing = resourceMap.find(uri);
    return existing != resourceMap.end() ? existing->second : nullptr;
}


template <typename TResource>
void ResourceManager::cleanUnusedResources(ResourceCollection<TResource>& resources)
{
    auto uris = std::unordered_set<std::string>();
    for (auto& it : resources)
    {
        if (it.second.use_count() == 1)
            uris.insert(it.first);
    }
    for (auto& uri : uris)
        resources.erase(uri);
}


void ResourceManager::cleanUnusedResources()
{
    // Clean in order of reference hierarchy
    cleanUnusedResources(frameBuffers);
    cleanUnusedResources(materials);
    cleanUnusedResources(effects);
    cleanUnusedResources(meshes);
    cleanUnusedResources(textures2d);
    cleanUnusedResources(cubeTextures);
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
