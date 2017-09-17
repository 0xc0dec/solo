/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <functional>

namespace solo
{
    template <class T>
    class AsyncHandle
    {
    public:
        void done(std::function<void(sptr<T>)> callback)
        {
            if (callback && result)
                callback(result);
            this->callback = callback;
        }

    private:
        friend class Texture2d; // TODO!
        friend class CubeTexture; // TODO!

        std::function<void(sptr<T>)> callback;
        sptr<T> result;

        void finish(sptr<T> result)
        {
            if (callback)
                callback(result);
        }
    };
}