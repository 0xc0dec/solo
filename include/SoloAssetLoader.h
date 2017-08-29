/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <vector>
#include <functional>

namespace solo
{
    class Device;
    class Mesh;
    class RectTexture;
    class CubeTexture;
    class ImageLoader;
    class MeshLoader;

    template <class T>
    class AsyncHandle
    {
    public:
        void done(std::function<void(sptr<T>)> callback)
        {
            if (callback && result)
                callback(result);
            this->callback = callback;
        }

    private:
        friend class AssetLoader;

        std::function<void(sptr<T>)> callback;
        sptr<T> result;

        void finish(sptr<T> result)
        {
            if (callback)
                callback(result);
        }
    };

    struct AssetLoaderImpl;

    class AssetLoader
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(AssetLoader)

        explicit AssetLoader(Device *device, const FriendToken<Device> &);
        ~AssetLoader() {}

        auto loadRectTexture(const std::string &path) -> sptr<RectTexture>;
        auto loadRectTextureAsync(const std::string &path) -> sptr<AsyncHandle<RectTexture>>;

        auto loadCubeTexture(const std::vector<std::string> &sidePaths) -> sptr<CubeTexture>;
        auto loadCubeTextureAsync(const std::vector<std::string> &sidePaths) -> sptr<AsyncHandle<CubeTexture>>;

        auto loadMesh(const std::string &path) -> sptr<Mesh>;
        auto loadMeshAsync(const std::string &path) -> sptr<AsyncHandle<Mesh>>;

        void update();

    private:
        sptr<AssetLoaderImpl> impl;
    };
}
