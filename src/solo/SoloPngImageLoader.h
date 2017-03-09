/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloImageLoader.h"

namespace solo
{
    class Device;
    class FileSystem;

    class PngImageLoader final: public ImageLoader
    {
    public:
        explicit PngImageLoader(Device *device);

        bool isLoadable(const std::string &path) const override final;
        auto load(const std::string &path) -> sptr<Image> override final;

    private:
        FileSystem *fs = nullptr;
    };
}