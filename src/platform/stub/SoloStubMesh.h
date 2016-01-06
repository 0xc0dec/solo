#pragma once

#include "SoloMesh.h"

namespace solo
{
    class StubMesh: public Mesh
    {
    public:
        virtual void draw() override {}
        virtual void setVertices(const std::vector<Vector3> &vertices) override {}
        virtual void setNormals(const std::vector<Vector3> &normals) override {}
        virtual void setUVs(const std::vector<Vector2> &uvs) override {}
        virtual void setIndices(const std::vector<unsigned short> &indices) override {}

    private:
        friend class Mesh;

        StubMesh() {}
    };
}