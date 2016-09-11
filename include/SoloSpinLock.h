/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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
            LockToken(std::atomic_flag& flag) : flag(flag)
            {
            }

            ~LockToken()
            {
                flag.clear();
            }

        private:
            std::atomic_flag& flag;
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
