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
        virtual auto load(const std::string& uri) -> uptr<Image> = 0;
    };
}