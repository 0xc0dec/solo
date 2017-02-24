/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <atomic>


namespace solo
{
    class SpinLock final
    {
    public:
        class LockToken final
        {
        public:
            explicit LockToken(std::atomic_flag &flag) : flag(flag)
            {
            }

            ~LockToken()
            {
                flag.clear();
            }

        private:
            std::atomic_flag &flag;
        };

        ~SpinLock()
        {
            flag.clear(std::memory_order_release);
        }

        auto acquire()
        {
            while (flag.test_and_set(std::memory_order_acquire)) {}
            return LockToken(flag);
        }

    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
    };
}
