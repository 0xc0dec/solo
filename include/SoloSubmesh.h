#pragma once

#include "SoloBase.h"

namespace solo
{
    class Mesh;

    class IndexedMeshPart
    {
    public:
        static shared<IndexedMeshPart> create();

        IndexedMeshPart(const IndexedMeshPart &other) = delete;
        IndexedMeshPart(IndexedMeshPart &&other) = delete;
        IndexedMeshPart &operator=(const IndexedMeshPart &other) = delete;
        IndexedMeshPart &operator=(IndexedMeshPart &&other) = delete;
        ~IndexedMeshPart() {}

    private:
        IndexedMeshPart() {}
    };
}