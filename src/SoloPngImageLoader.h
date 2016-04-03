#pragma once

#include "SoloImageLoader.h"


namespace solo
{
    class PngImageLoader : public ImageLoader
    {
    public:
        virtual bool isLoadable(const std::string& uri) override final;
        virtual auto load(const std::string& uri) -> uptr<Image> override final;
    };
}