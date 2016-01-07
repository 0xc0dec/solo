#pragma once

#include "SoloBase.h"

namespace solo
{
    class Mesh2;
    class FileSystem;
    class ResourceManager;

    class MeshLoader
    {
    public:
        SL_NONCOPYABLE(MeshLoader);
        virtual ~MeshLoader() {}

        virtual bool isLoadable(const std::string &uri) = 0;
        virtual shared<Mesh2> load(const std::string &uri) = 0;

    protected:
        MeshLoader(FileSystem *fs, ResourceManager *resourceManager):
            fs(fs),
            resourceManager(resourceManager)
        {
        }

        FileSystem *fs;
        ResourceManager *resourceManager;
    };
}