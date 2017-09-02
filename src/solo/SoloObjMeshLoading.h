/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <string>

namespace solo
{
    class Mesh;
    class Device;

    namespace obj
    {
        bool canLoad(const std::string &path);
        auto loadMesh(Device *device, const std::string &path) -> sptr<Mesh>;
    }
}