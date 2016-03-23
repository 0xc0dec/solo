#pragma once

#include "../../SoloMeshLoader.h"


namespace solo
{
    class StubMeshLoader final: public MeshLoader
    {
    public:
        StubMeshLoader(FileSystem* fs, ResourceManager* resourceManager);

        virtual bool isLoadable(const std::string& uri) override final;
        virtual shared<Mesh> load(const std::string& uri) override final;
        virtual unique<MeshData> loadData(const std::string& uri) override final;
    };
}