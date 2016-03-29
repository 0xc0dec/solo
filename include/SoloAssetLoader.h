#pragma once

#include "SoloBase.h"
#include "SoloSpinLock.h"
#include <vector>
#include <list>
#include <functional>


namespace solo
{
    class DeviceToken;
    class Mesh;
    class Texture2D;
    class CubeTexture;
    class ImageLoader;
    class MeshLoader;

    template <class T>
    class AsyncHandle
    {
    public:
        sptr<T> getResult()
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

        void putResult(sptr<T> result)
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
        AssetLoader(const DeviceToken&);
        ~AssetLoader();
        SL_NONCOPYABLE(AssetLoader)

        sptr<Texture2D> loadTexture2D(const std::string& path);
        sptr<AsyncHandle<Texture2D>> loadTexture2DAsync(const std::string& path);
        
        sptr<CubeTexture> loadCubeTexture(const std::vector<std::string>& sidesPaths);
        sptr<AsyncHandle<CubeTexture>> loadCubeTextureAsync(const std::vector<std::string>& sidesPaths);

        sptr<Mesh> loadMesh(const std::string& path);
        sptr<AsyncHandle<Mesh>> loadMeshAsync(const std::string& path);

        void update();

    private:
        std::vector<uptr<ImageLoader>> imageLoaders;
        std::vector<uptr<MeshLoader>> meshLoaders;

        MeshLoader* getMeshLoader(const std::string& path);
        ImageLoader* getImageLoader(const std::string& path);

        SpinLock tasksLock;
        std::list<std::function<void()>> tasks;
    };
}