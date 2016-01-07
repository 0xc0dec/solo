#pragma once

#include "SoloBase.h"

namespace solo
{
    class Mesh;

    class Submesh
    {
    public:
        static shared<Submesh> create();

        Submesh(const Submesh &other) = delete;
        Submesh(Submesh &&other) = delete;
        Submesh &operator=(const Submesh &other) = delete;
        Submesh &operator=(Submesh &&other) = delete;
        ~Submesh() {}

    private:
        Submesh() {}
    };
}