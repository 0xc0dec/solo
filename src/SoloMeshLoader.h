#pragma once

#include "SoloBase.h"


namespace solo
{
    class Mesh;
    struct MeshData;
    class FileSystem;
    class ResourceManager;

    class MeshLoader
    {
    public:
        SL_NONCOPYABLE(MeshLoader);
        virtual ~MeshLoader() {}

        virtual bool isLoadable(const std::string& uri) = 0;
        virtual sptr<Mesh> load(const std::string& uri) = 0;
        virtual uptr<MeshData> loadData(const std::string& uri) = 0;

    protected:
        MeshLoader(FileSystem* fs, ResourceManager* resourceManager):
            fs(fs),
            resourceManager(resourceManager)
        {
        }

        FileSystem* fs;
        ResourceManager* resourceManager;
    };
}