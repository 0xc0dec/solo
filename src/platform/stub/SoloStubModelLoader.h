#pragma once

#include "../../SoloMeshLoader.h"

namespace solo
{
    class StubMeshLoader: public MeshLoader
    {
    public:
        StubMeshLoader(FileSystem *fs, ResourceManager *resourceManager);

        virtual bool isLoadable(const std::string &uri) override;
        virtual shared<Mesh> load(const std::string &uri) override;
    };
}