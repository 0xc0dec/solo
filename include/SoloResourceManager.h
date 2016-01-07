#pragma once

#include "SoloBase.h"

namespace solo
{
    class Effect;
    class Material;
    class Mesh2;
    class Texture2D;
    class CubeTexture;
    class RenderTarget;
    class ImageLoader;
    class MeshLoader;
    class Device;

    struct KnownUris
    {
        static constexpr char *SkyboxEffect = "solo/internal/effects/skybox";
        static constexpr char *UnitQuadMesh = "solo/internal/meshes/quad";
    };

    class ResourceManager
    {
    public:
        static shared<ResourceManager> create(Device *device);

        ResourceManager(const ResourceManager &other) = delete;
        ResourceManager(ResourceManager &&other) = delete;
        ResourceManager &operator=(const ResourceManager &other) = delete;
        ResourceManager &operator=(ResourceManager &&other) = delete;
        ~ResourceManager() {}

        shared<Effect> findEffect(const std::string &uri);
        shared<Texture2D> findTexture2D(const std::string &uri);
        shared<CubeTexture> findCubeTexture(const std::string &uri);
        shared<Material> findMaterial(const std::string &uri);
        shared<Mesh2> findMesh(const std::string &uri);
        shared<RenderTarget> findRenderTarget(const std::string &uri);

        shared<Effect> getOrCreateEffect(const std::string &vsSrc, const std::string &fsSrc, const std::string &uri = "");
        shared<Texture2D> getOrCreateTexture2D(const std::string &uri = "");
        shared<CubeTexture> getOrCreateCubeTexture(const std::string &uri = "");
        shared<Material> getOrCreateMaterial(shared<Effect> effect, const std::string &uri = "");
        shared<Mesh2> getOrCreateMesh(const std::string &uri = "");
        shared<RenderTarget> getOrCreateRenderTarget(const std::string &uri = "");

        shared<Texture2D> getOrLoadTexture2D(const std::string &imageUri, const std::string &uri = "");
        shared<CubeTexture> getOrLoadCubeTexture(const std::vector<std::string> &imageUris, const std::string &uri = "");
        shared<Mesh2> getOrLoadMesh(const std::string &dataUri, const std::string &uri = "");

        void cleanUnusedResources();

    protected:
        explicit ResourceManager(Device *device);

        std::vector<shared<ImageLoader>> imageLoaders;
        std::vector<shared<MeshLoader>> meshLoaders;

    private:
        template <typename TResource> using ResourceMap = std::unordered_map<std::string, shared<TResource>>;

        std::string generateUri();

        template <typename TResource>
        static void cleanUnusedResources(ResourceMap<TResource> &resources);

        template <typename TResource>
        shared<TResource> getOrCreateResource(const std::string &uri, ResourceMap<TResource> &resourceMap,
                                              std::function<shared<TResource>(const std::basic_string<char>&)> find,
                                              std::function<shared<TResource>()> create);

        template <typename TResource>
        shared<TResource> createResource(const std::string &uri, ResourceMap<TResource> &resourceMap,
                                         std::function<shared<TResource>()> create);

        template <typename TResource>
        shared<TResource> findResource(const std::string &uri, const ResourceMap<TResource> &resourceMap);

        shared<Effect> tryCreateBuiltInEffect(const std::string &uri);
        shared<Mesh2> tryCreateBuiltInMesh(const std::string &uri);

        Device *device{ nullptr };

        ResourceMap<Effect> effects;
        ResourceMap<Material> materials;
        ResourceMap<Mesh2> meshes;
        ResourceMap<Texture2D> textures2d;
        ResourceMap<CubeTexture> cubeTextures;
        ResourceMap<RenderTarget> renderTargets;

        size_t resourceCounter = 0;
    };
}