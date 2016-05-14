#pragma once

#include "SoloImageLoader.h"


namespace solo
{
    class PngImageLoader : public ImageLoader
    {
    public:
        virtual bool isLoadable(const std::string& path) override final;
        virtual auto load(const std::string& path) -> sptr<Image> override final;
    };
}