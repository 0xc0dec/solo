#pragma once

#include "SoloBase.h"


namespace solo
{
    struct MeshData;

    class MeshLoader
    {
    public:
        MeshLoader() {}
        virtual ~MeshLoader() {}
        SL_NONCOPYABLE(MeshLoader);

        virtual bool isLoadable(const std::string& uri) = 0;
        virtual uptr<MeshData> loadData(const std::string& uri) = 0;
    };
}