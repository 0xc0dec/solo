#pragma once

#include "SoloImageLoader.h"


namespace solo
{
    class PngImageLoader : public ImageLoader
    {
    public:
        bool isLoadable(const std::string& path) const override final;
        auto load(const std::string& path) -> sptr<Image> override final;
    };
}