#pragma once

#include "SoloModelLoader.h"

namespace solo
{
    class ObjModelLoader: public ModelLoader
    {
    public:
        ObjModelLoader(FileSystem *fs, ResourceManager *resourceManager);

        virtual bool isLoadable(const std::string &uri) override;
        virtual shared<Model> load(const std::string &uri) override;
    };
}