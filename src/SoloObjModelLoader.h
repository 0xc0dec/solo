#pragma once

#include "SoloModelLoader.h"

namespace solo
{
    class ObjMeshLoader: public MeshLoader
    {
    public:
        ObjMeshLoader(FileSystem *fs, ResourceManager *resourceManager);

        virtual bool isLoadable(const std::string &uri) override;
        virtual shared<Mesh2> load(const std::string &uri) override;
    };
}