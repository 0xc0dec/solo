#pragma once

#include "SoloMeshLoader.h"


namespace solo
{
    class ObjMeshLoader final: public MeshLoader
    {
    public:
        virtual bool isLoadable(const std::string& uri) override final;
        virtual uptr<MeshData> loadData(const std::string& uri) override final;
    };
}