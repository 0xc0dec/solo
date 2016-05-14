#pragma once

#include "SoloBase.h"


namespace solo
{
    struct Image;

    class ImageLoader
    {
    public:
        virtual ~ImageLoader() {}
        SL_NONCOPYABLE(ImageLoader)

        virtual bool isLoadable(const std::string& path) = 0;
        virtual auto load(const std::string& path) -> sptr<Image> = 0;

    protected:
        ImageLoader() {}
    };
}