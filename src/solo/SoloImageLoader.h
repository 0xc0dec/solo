/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"


namespace solo
{
    struct Image;

    class ImageLoader
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(ImageLoader)

        virtual ~ImageLoader() {}

        virtual bool isLoadable(const std::string &path) const = 0;
        virtual auto load(const std::string &path) -> sptr<Image> = 0;

    protected:
        ImageLoader() {}
    };
}