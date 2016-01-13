#pragma once

#include "SoloMeshLoader.h"

namespace solo
{
    class ObjMeshLoader: public MeshLoader
    {
    public:
        ObjMeshLoader(FileSystem* fs, ResourceManager* resourceManager);

        virtual bool isLoadable(const std::string& uri) override;
        virtual shared<Mesh> load(const std::string& uri) override;
    };
}