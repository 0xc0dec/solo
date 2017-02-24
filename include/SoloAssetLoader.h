/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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
