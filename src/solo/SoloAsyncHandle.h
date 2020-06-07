/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
            if (callback && result_)
                callback(result_);
            this->callback_ = callback;
        }

        void resolve(sptr<T> result)
        {
            if (callback_)
                callback_(result);
            this->result_ = result;
        }

    private:
        std::function<void(sptr<T>)> callback_;
        sptr<T> result_;
    };
}