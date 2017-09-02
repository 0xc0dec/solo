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

    struct AssetLoaderImpl;

    class AssetLoader
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(AssetLoader)

        explicit AssetLoader(Device *device, const FriendToken<Device> &);
        ~AssetLoader() {}

        auto loadRectTexture(const std::string &path) -> sptr<RectTexture>;
        auto loadCubeTexture(const std::vector<std::string> &sidePaths) -> sptr<CubeTexture>;
        auto loadMesh(const std::string &path) -> sptr<Mesh>;

    private:
        sptr<AssetLoaderImpl> impl;
    };
}
