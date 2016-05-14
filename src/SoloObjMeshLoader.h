#pragma once

#include "SoloMeshLoader.h"


namespace solo
{
    class ObjMeshLoader final: public MeshLoader
    {
    public:
        virtual bool isLoadable(const std::string& path) override final;
        virtual auto loadData(const std::string& path) -> sptr<MeshData> override final;
    };
}