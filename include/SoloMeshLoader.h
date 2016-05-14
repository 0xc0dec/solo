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

        virtual bool isLoadable(const std::string& path) = 0;
        virtual auto loadData(const std::string& path) -> sptr<MeshData> = 0;
    };
}