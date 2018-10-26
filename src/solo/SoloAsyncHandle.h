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
            if (callback && result_ && !invoked_)
            {
                callback(result_);
                invoked_ = true;
            }
            this->callback_ = callback;
        }

        void resolve(sptr<T> result)
        {
            if (callback_)
            {
                callback_(result);
                invoked_ = true;
            }
            this->result_ = result;
        }

    private:
        bool invoked_ = false;
        std::function<void(sptr<T>)> callback_;
        sptr<T> result_;
    };
}