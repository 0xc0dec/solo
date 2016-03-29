#pragma once

#include "SoloImageLoader.h"


namespace solo
{
    class PngImageLoader : public ImageLoader
    {
    public:
        virtual bool isLoadable(const std::string& uri) override final;
        virtual uptr<Image> load(const std::string& uri) override final;
    };
}