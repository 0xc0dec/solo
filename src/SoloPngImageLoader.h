#pragma once

#include "SoloImageLoader.h"


namespace solo
{
    class PngImageLoader : public ImageLoader
    {
    public:
        PngImageLoader(FileSystem* fs, ResourceManager* resourceManager);

        virtual bool isLoadable(const std::string& uri) override final;
        virtual unique<Image> load(const std::string& uri) override final;
    };
}