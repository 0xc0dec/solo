#pragma once

#include "SoloCommon.h"
#include "SoloSpinLock.h"
#include <vector>
#include <list>
#include <functional>


namespace solo
{
    class DeviceToken;
    class Mesh;
    class RectTexture;
    class CubeTexture;
    class ImageLoader;
    class MeshLoader;

    template <class T>
    class AsyncHandle
    {
    public:
        auto getResult()
        {
            return result;
        }

        void done(std::function<void(sptr<T>)> callback)
        {
            if (callback && result)
                callback(result);
            this->callback = callback;
        }

    private:
        friend class AssetLoader;

        void finish(sptr<T> result)
        {
            if (callback)
                callback(result);
        }

        std::function<void(sptr<T>)> callback;
        sptr<T> result;
    };


    class AssetLoader
    {
    public:
        explicit AssetLoader(const DeviceToken&);
        ~AssetLoader();
        SL_NONCOPYABLE(AssetLoader)

        auto loadRectTexture(const std::string& path) -> sptr<RectTexture>;
        auto loadRectTextureAsync(const std::string& path) -> sptr<AsyncHandle<RectTexture>>;
        
        auto loadCubeTexture(const std::vector<std::string>& sidePaths) -> sptr<CubeTexture>;
        auto loadCubeTextureAsync(const std::vector<std::string>& sidePaths) -> sptr<AsyncHandle<CubeTexture>>;

        auto loadMesh(const std::string& path) -> sptr<Mesh>;
        auto loadMeshAsync(const std::string& path) -> sptr<AsyncHandle<Mesh>>;

        void update();

    private:
        std::vector<uptr<ImageLoader>> imageLoaders;
        std::vector<uptr<MeshLoader>> meshLoaders;

        auto getMeshLoader(const std::string& path) -> MeshLoader*;
        auto getImageLoader(const std::string& path) -> ImageLoader*;

        SpinLock tasksLock;
        std::list<std::function<void()>> tasks;
    };
}
