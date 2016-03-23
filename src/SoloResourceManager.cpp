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
#include "SoloLogger.h"
#include "platform/stub/SoloStubResourceManager.h"
#include <functional>


namespace solo
{
    using LoadMeshUserCallback = std::function<void(shared<Mesh>)>;
    using LoadMeshSystemCallback = std::function<shared<Mesh>(unique<MeshData>, const std::string&)>;

    class Task
    {
    public:
        virtual ~Task() {}
        virtual void process() = 0;
        virtual void finish() = 0;
    };

    class LoadMeshTask final: public Task
    {
    public:
        LoadMeshTask(const std::string& dataUri, const std::string& meshUri, MeshLoader* loader,
            LoadMeshSystemCallback systemCallback, LoadMeshUserCallback userCallback):
            loader(loader),
            dataUri(dataUri),
            meshUri(meshUri),
            systemCallback(systemCallback),
            userCallback(userCallback)
        {
        }

        virtual void process() override final
        {
            meshData = std::move(loader->loadData(dataUri));
        }

        virtual void finish() override final
        {
            auto mesh = systemCallback(std::move(meshData), meshUri);
            userCallback(mesh);
        }

    private:
        MeshLoader* loader;
        unique<MeshData> meshData;
        std::string dataUri;
        std::string meshUri;
        LoadMeshSystemCallback systemCallback;
        LoadMeshUserCallback userCallback;
    };
}

using namespace solo;


shared<ResourceManager> ResourceManager::create(Device* device, int workersCount)
{
    if (device->getMode() == DeviceMode::Stub)
        return SL_NEW_SHARED(StubResourceManager, device);
    return SL_NEW_SHARED(ResourceManager, device, workersCount);
}


ResourceManager::ResourceManager(Device* device, int workersCount):
    device(device)
{
    imageLoaders.push_back(SL_MAKE_UNIQUE<PngImageLoader>(device->getFileSystem(), this));
    meshLoaders.push_back(SL_MAKE_UNIQUE<ObjMeshLoader>(device->getFileSystem(), this));

    for (int32_t i = 0; i < workersCount; i++)
        workers.push_back(std::thread(&ResourceManager::runWorker, this));
}


ResourceManager::~ResourceManager()
{
    stopWorkers = true;
    workersSignal.notify_all();
    for (auto& t : workers)
        t.join();
}


std::string ResourceManager::generateUri()
{
    return std::string("/generated/") + std::to_string(++resourceCounter);
}


void ResourceManager::runWorker()
{
    while (true)
    {
        unique<Task> task;
        {
            std::unique_lock<std::mutex> lock(workersMutex);
            workersSignal.wait(lock, [&]() { return !backgroundTasks.empty() || stopWorkers; });
            if (stopWorkers)
                break;
            task = std::move(backgroundTasks.front());
            backgroundTasks.pop_front();
        }
        if (!task)
            continue;

        {
            auto lock = foregroundTasksLock.lock();
            task->process();
            foregroundTasks.push_back(std::move(task));
        }
    }
}


shared<Mesh> ResourceManager::processLoadedMeshData(unique<MeshData> data, const std::string& meshUri)
{
    // TODO this effectively rewrites anything that might have been put by this uri
    // while the mesh data has been loading. Probably ok for now
    auto mesh = SL_MAKE_SHARED<Mesh>(device->getRenderer(), data.get());
    meshes[meshUri] = mesh;
    return mesh;
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


shared<Effect> ResourceManager::findEffect(const std::string& uri)
{
    return findResource(uri, effects);
}


shared<Material> ResourceManager::findMaterial(const std::string& uri)
{
    return findResource(uri, materials);
}


shared<Texture2D> ResourceManager::findTexture2D(const std::string& uri)
{
    return findResource(uri, textures2d);
}


shared<CubeTexture> ResourceManager::findCubeTexture(const std::string& uri)
{
    return findResource(uri, cubeTextures);
}


shared<Mesh> ResourceManager::findMesh(const std::string& uri)
{
    return findResource(uri, meshes);
}


shared<FrameBuffer> ResourceManager::findFrameBuffer(const std::string& uri)
{
    return findResource(uri, frameBuffers);
}


shared<Effect> ResourceManager::getOrCreateEffect(const std::string& vsSrc, const std::string& fsSrc, const std::string& uri)
{
    return getOrCreateResource<Effect>(uri, effects,
        std::bind(&ResourceManager::findEffect, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Effect, device->getRenderer(), vsSrc, fsSrc); });
}


shared<Effect> ResourceManager::getOrCreatePrefabEffect(EffectPrefab prefab, const std::string& uri)
{
    return getOrCreateResource<Effect>(uri, effects,
        std::bind(&ResourceManager::findEffect, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Effect, device->getRenderer(), prefab); });
}


shared<Material> ResourceManager::getOrCreateMaterial(shared<Effect> effect, const std::string& uri)
{
    return getOrCreateResource<Material>(uri, materials,
        std::bind(&ResourceManager::findMaterial, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Material, device->getRenderer(), effect); });
}


shared<Texture2D> ResourceManager::getOrCreateTexture2D(const std::string& uri)
{
    return getOrCreateResource<Texture2D>(uri, textures2d,
        std::bind(&ResourceManager::findTexture2D, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Texture2D, device->getRenderer()); });
}


shared<CubeTexture> ResourceManager::getOrCreateCubeTexture(const std::string& uri)
{
    return getOrCreateResource<CubeTexture>(uri, cubeTextures,
        std::bind(&ResourceManager::findCubeTexture, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(CubeTexture, device->getRenderer()); });
}


shared<Texture2D> ResourceManager::getOrLoadTexture2D(const std::string& imageUri, const std::string& uri)
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


shared<CubeTexture> ResourceManager::getOrLoadCubeTexture(const std::vector<std::string>& imageUris, const std::string& uri)
{
    auto textureUri = uri.empty()
        ? imageUris[0] + imageUris[1] + imageUris[2] + imageUris[3] + imageUris[4] + imageUris[5]
        : uri;
    auto existing = findCubeTexture(textureUri);
    if (existing)
        return existing;

    auto result = SL_NEW_SHARED(CubeTexture, device->getRenderer());
    auto idx = 0;
    for (auto& imageUri : imageUris)
    {
        shared<Image> image;
        for (const auto& loader : imageLoaders)
        {
            if (loader->isLoadable(imageUri))
            {
                image = loader->load(imageUri);
                auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + idx);
                result->setData(face, image->colorFormat, image->data, image->width, image->height);
                break;
            }
        }
        if (!image)
            SL_FMT_THROW(ResourceException, "No suitable loader found for image ", imageUri);
        idx++;
    }

    cubeTextures[textureUri] = result;
    return result;
}


shared<Mesh> ResourceManager::getOrLoadMesh(const std::string& dataUri, const std::string& uri)
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


void ResourceManager::getOrLoadMeshAsync(const std::string& dataUri, std::function<void(shared<Mesh>)> callback, const std::string& uri)
{
    auto meshUri = uri.empty() ? dataUri : uri;
    auto existing = findMesh(meshUri);
    if (existing)
    {
        callback(existing);
        return;
    }

    auto loader = getMeshLoader(dataUri);

    {
        std::unique_lock<std::mutex> lock(workersMutex);
        auto task = SL_MAKE_UNIQUE<LoadMeshTask>(dataUri, meshUri, loader,
            std::bind(&ResourceManager::processLoadedMeshData, this, std::placeholders::_1, std::placeholders::_2),
            callback);
        backgroundTasks.push_back(std::move(task));
    }
    workersSignal.notify_all();
}


shared<Mesh> ResourceManager::getOrCreateMesh(const std::string& uri)
{
    return getOrCreateResource<Mesh>(uri, meshes,
        std::bind(&ResourceManager::findMesh, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Mesh, device->getRenderer()); });
}


shared<Mesh> ResourceManager::getOrCreatePrefabMesh(MeshPrefab prefab, const std::string& uri)
{
    return getOrCreateResource<Mesh>(uri, meshes,
        std::bind(&ResourceManager::findMesh, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(Mesh, device->getRenderer(), prefab); });
}


shared<FrameBuffer> ResourceManager::getOrCreateFrameBuffer(const std::string& uri)
{
    return getOrCreateResource<FrameBuffer>(uri, frameBuffers,
        std::bind(&ResourceManager::findFrameBuffer, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(FrameBuffer, device->getRenderer()); });
}


template <typename TResource>
shared<TResource> ResourceManager::getOrCreateResource(
    const std::string& uri,
    ResourceCollection<TResource>& resourceMap,
    std::function<shared<TResource>(const std::basic_string<char>&)> find,
    std::function<shared<TResource>()> create)
{
    auto existing = find(uri.empty() ? generateUri() : uri); // TODO use findResource here?
    return existing ? existing : createResource<TResource>(uri, resourceMap, create);
}


template <typename TResource>
shared<TResource> ResourceManager::createResource(const std::string& uri, ResourceCollection<TResource>& resourceMap, std::function<shared<TResource>()> create)
{
    auto result = create();
    resourceMap[uri] = result;
    return result;
}


template <typename TResource>
shared<TResource> ResourceManager::findResource(const std::string& uri, const ResourceCollection<TResource>& resourceMap)
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
    if (!foregroundTasks.empty())
    {
        auto lt = foregroundTasksLock.lock();
        if (!foregroundTasks.empty())
        {
            auto task = std::move(foregroundTasks.back());
            task->finish();
            foregroundTasks.pop_back();
        }
    }
}
