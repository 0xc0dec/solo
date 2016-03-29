#pragma once

#include "SoloBase.h"


namespace solo
{
    struct Image;

    class ImageLoader
    {
    public:
        ImageLoader() {}
        virtual ~ImageLoader() {}
        SL_NONCOPYABLE(ImageLoader)

        virtual bool isLoadable(const std::string& uri) = 0;
        virtual uptr<Image> load(const std::string& uri) = 0;
    };
}