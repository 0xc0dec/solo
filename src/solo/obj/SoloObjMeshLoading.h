/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloAsyncHandle.h"

namespace solo
{
    class Mesh;
    class Device;

    namespace obj
    {
        bool canLoadMesh(const str &path);
        auto loadMesh(Device *device, const str &path) -> sptr<Mesh>;
        auto loadMeshAsync(Device *device, const str &path) -> sptr<AsyncHandle<Mesh>>;
    }
}