/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloMeshLoader.h"

namespace solo
{
    class Device;
    class FileSystem;

    class ObjMeshLoader final: public MeshLoader
    {
    public:
        explicit ObjMeshLoader(Device *device);

        bool isLoadable(const std::string &path) const override final;
        auto loadData(const std::string &path) const -> sptr<MeshData> override final;

    private:
        FileSystem *fs = nullptr;
    };
}