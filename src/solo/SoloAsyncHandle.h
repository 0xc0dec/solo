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
            if (callback && result && !invoked)
            {
                callback(result);
                invoked = true;
            }
            this->callback = callback;
        }

        void finish(sptr<T> result)
        {
            if (callback)
            {
                callback(result);
                invoked = true;
            }
            this->result = result;
        }

    private:
        bool invoked = false;
        std::function<void(sptr<T>)> callback;
        sptr<T> result;
    };
}