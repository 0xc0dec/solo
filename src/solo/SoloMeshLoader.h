/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

namespace solo
{
    struct MeshData;

    class MeshLoader
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(MeshLoader)

        virtual ~MeshLoader() {}

        virtual bool isLoadable(const str &path) const = 0;
        virtual auto loadData(const str &path) const -> sptr<MeshData> = 0;

    protected:
        MeshLoader() {}
    };
}