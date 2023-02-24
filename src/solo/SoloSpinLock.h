/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

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
            flag_.clear(std::memory_order_release);
        }

        auto acquire()
        {
            while (flag_.test_and_set(std::memory_order_acquire)) {}
            return LockToken(flag_);
        }

    private:
        std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
    };
}
