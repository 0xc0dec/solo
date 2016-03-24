#pragma once

#include "SoloBase.h"
#include "SoloSpinLock.h"
#include <vector>
#include <unordered_map>
#include <functional>


namespace solo
{
    class Effect;
    class Material;
    class Mesh;
    class Texture2D;
    class CubeTexture;
    class FrameBuffer;
    class ImageLoader;
    class MeshLoader;
    class Device;
    struct MeshData;
    enum class MeshPrefab;
    enum class EffectPrefab;

    template <class T>
    class AsyncResourceHandle
    {
    public:
        using CallbackType = std::function<void(sptr<T>)>;

        void done(CallbackType callback)
        {
            if (callback && result)
                callback(result);
            this->callback = callback;
        }

    private:
        friend class ResourceManager;

        void putResult(sptr<T> result)
        {
            if (callback)
                callback(result);
        }

        sptr<T> result;
        CallbackType callback;
    };


    class ResourceManager
    {
    public:
        static sptr<ResourceManager> create(Device* device);

        SL_NONCOPYABLE(ResourceManager)
        ~ResourceManager();

        sptr<Effect> findEffect(const std::string& uri);
        sptr<Texture2D> findTexture2D(const std::string& uri);
        sptr<CubeTexture> findCubeTexture(const std::string& uri);
        sptr<Material> findMaterial(const std::string& uri);
        sptr<Mesh> findMesh(const std::string& uri);
        sptr<FrameBuffer> findFrameBuffer(const std::string& uri);

        sptr<Effect> getOrCreateEffect(const std::string& vsSrc, const std::string& fsSrc, const std::string& uri = "");
        sptr<Effect> getOrCreatePrefabEffect(EffectPrefab prefab, const std::string& uri = "");
        sptr<Texture2D> getOrCreateTexture2D(const std::string& uri = "");
        sptr<CubeTexture> getOrCreateCubeTexture(const std::string& uri = "");
        sptr<Material> getOrCreateMaterial(sptr<Effect> effect, const std::string& uri = "");
        sptr<Mesh> getOrCreateMesh(const std::string& uri = "");
        sptr<Mesh> getOrCreatePrefabMesh(MeshPrefab prefab, const std::string& uri = "");
        sptr<FrameBuffer> getOrCreateFrameBuffer(const std::string& uri = "");

        sptr<Texture2D> getOrLoadTexture2D(const std::string& imageUri, const std::string& uri = "");
        
        sptr<CubeTexture> getOrLoadCubeTexture(const std::vector<std::string>& imageUris, const std::string& uri = "");
        sptr<AsyncResourceHandle<CubeTexture>> getOrLoadCubeTextureAsync(const std::vector<std::string>& sidesUris, const std::string& uri = "");

        sptr<Mesh> getOrLoadMesh(const std::string& dataUri, const std::string& uri = "");
        sptr<AsyncResourceHandle<Mesh>> getOrLoadMeshAsync(const std::string& dataUri, const std::string& uri = "");

        void cleanUnusedResources();

        void update();

    protected:
        explicit ResourceManager(Device* device);

        std::vector<uptr<ImageLoader>> imageLoaders;
        std::vector<uptr<MeshLoader>> meshLoaders;

    private:
        template <typename TResource> using ResourceCollection = std::unordered_map<std::string, sptr<TResource>>;

        std::string generateUri();
        MeshLoader* getMeshLoader(const std::string& uri);
        ImageLoader* getImageLoader(const std::string& uri);

        template <typename TResource>
        static void cleanUnusedResources(ResourceCollection<TResource>& resources);

        template <typename TResource>
        sptr<TResource> getOrCreateResource(const std::string& uri, ResourceCollection<TResource>& resourceMap,
            std::function<sptr<TResource>(const std::basic_string<char>&)> find,
            std::function<sptr<TResource>()> create);

        template <typename TResource>
        sptr<TResource> createResource(const std::string& uri, ResourceCollection<TResource>& resourceMap,
            std::function<sptr<TResource>()> create);

        template <typename TResource>
        sptr<TResource> findResource(const std::string& uri, const ResourceCollection<TResource>& resourceMap);

        Device* device = nullptr;

        ResourceCollection<Effect> effects;
        ResourceCollection<Material> materials;
        ResourceCollection<Mesh> meshes;
        ResourceCollection<Texture2D> textures2d;
        ResourceCollection<CubeTexture> cubeTextures;
        ResourceCollection<FrameBuffer> frameBuffers;

        uint32_t resourceCounter = 0;

        SpinLock tasksLock;
        std::list<std::function<void()>> tasks;
    };
}