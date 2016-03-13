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
#include "SoloSurfaceRenderer.h"
#include "SoloImage.h"
#include "platform/stub/SoloStubResourceManager.h"
#include <functional>

using namespace solo;


shared<ResourceManager> ResourceManager::create(Device* device)
{
    if (device->getMode() == DeviceMode::Stub)
        return SL_NEW_SHARED(StubResourceManager, device);
    return SL_NEW_SHARED(ResourceManager, device);
}


ResourceManager::ResourceManager(Device* device):
    device(device)
{
    imageLoaders.push_back(SL_MAKE_SHARED<PngImageLoader>(device->getFileSystem(), this));
    meshLoaders.push_back(SL_MAKE_SHARED<ObjMeshLoader>(device->getFileSystem(), this));
}


std::string ResourceManager::generateUri()
{
    return std::string("generated/") + std::to_string(++resourceCounter);
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


shared<SurfaceRenderer> ResourceManager::findSurfaceRenderer(const std::string& uri)
{
    return findResource(uri, surfaceRenderers);
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


shared<Texture2D> ResourceManager::getOrLoadTexture2D(const std::string& imageUri, const std::string& uri)
{
    auto textureUri = uri.empty() ? imageUri : uri;
    auto existing = findTexture2D(textureUri);
    if (existing)
        return existing;

    for (auto loader : imageLoaders)
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
        for (auto loader : imageLoaders)
        {
            if (loader->isLoadable(imageUri))
            {
                image = loader->load(imageUri);
                auto face = static_cast<CubeTextureFace>(static_cast<int>(CubeTextureFace::Front) + idx);
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


shared<Mesh> ResourceManager::getOrLoadMesh(const std::string& dataUri, const std::string& uri)
{
    auto meshUri = uri.empty() ? dataUri : uri;
    auto existing = findMesh(meshUri);
    if (existing)
        return existing;

    for (auto loader : meshLoaders)
    {
        if (loader->isLoadable(dataUri))
        {
            auto mesh = loader->load(dataUri);
            meshes[meshUri] = mesh;
            return mesh;
        }
    }

    SL_FMT_THROW(ResourceException, "No suitable loader found for mesh ", dataUri);
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
        std::bind(&Mesh::createPrefab, device->getRenderer(), prefab));
}


shared<FrameBuffer> ResourceManager::getOrCreateFrameBuffer(const std::string& uri)
{
    return getOrCreateResource<FrameBuffer>(uri, frameBuffers,
        std::bind(&ResourceManager::findFrameBuffer, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(FrameBuffer, device->getRenderer()); });
}


shared<SurfaceRenderer> ResourceManager::getOrCreateSurfaceRenderer(shared<Material> material, const std::string& uri)
{
    return getOrCreateResource<SurfaceRenderer>(uri, surfaceRenderers,
        std::bind(&ResourceManager::findSurfaceRenderer, this, std::placeholders::_1),
        [&]() { return SL_NEW_SHARED(SurfaceRenderer, device->getRenderer(), material); });
}


template <typename TResource>
shared<TResource> ResourceManager::getOrCreateResource(
    const std::string& uri,
    ResourceMap<TResource>& resourceMap,
    std::function<shared<TResource>(const std::basic_string<char>&)> find,
    std::function<shared<TResource>()> create)
{
    auto existing = find(uri.empty() ? generateUri() : uri);
    return existing ? existing : createResource<TResource>(uri, resourceMap, create);
}


template <typename TResource>
shared<TResource> ResourceManager::createResource(const std::string& uri, ResourceMap<TResource>& resourceMap, std::function<shared<TResource>()> create)
{
    auto result = create();
    resourceMap[uri] = result;
    return result;
}


template <typename TResource>
shared<TResource> ResourceManager::findResource(const std::string& uri, const ResourceMap<TResource>& resourceMap)
{
    auto existing = resourceMap.find(uri);
    return existing != resourceMap.end() ? existing->second : nullptr;
}


template <typename TResource>
void ResourceManager::cleanUnusedResources(ResourceMap<TResource>& resources)
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
    cleanUnusedResources(surfaceRenderers);
    cleanUnusedResources(frameBuffers);
    cleanUnusedResources(materials);
    cleanUnusedResources(effects);
    cleanUnusedResources(meshes);
    cleanUnusedResources(textures2d);
    cleanUnusedResources(cubeTextures);
}