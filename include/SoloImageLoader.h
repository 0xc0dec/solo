#pragma once

#include "SoloBase.h"

namespace solo
{
    struct Image;
    class FileSystem;
    class ResourceManager;

    class ImageLoader
    {
    public:
        SL_NONCOPYABLE(ImageLoader);
        virtual ~ImageLoader() {}

        virtual bool isLoadable(const std::string& uri) = 0;
        virtual shared<Image> load(const std::string& uri) = 0;

    protected:
        ImageLoader(FileSystem* fs, ResourceManager* resourceManager):
            fs(fs),
            resourceManager(resourceManager)
        {
        }

        FileSystem* fs;
        ResourceManager* resourceManager;
    };
}