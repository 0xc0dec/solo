#pragma once

#include <atomic>


namespace solo
{
    // TODO this is not a "lock", a "lock" is rather the "LockToken" below
    class SpinLock
    {
    public:
        class LockToken
        {
        public:
            LockToken(std::atomic_flag& flag) : flagRef(flag)
            {
            }

            ~LockToken()
            {
                flagRef.clear();
            }

        private:
            std::atomic_flag& flagRef;
        };

        ~SpinLock()
        {
            flag.clear(std::memory_order_release);
        }

        LockToken lock()
        {
            while (flag.test_and_set(std::memory_order_acquire)) {}
            return LockToken(flag);
        }

    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
    };
}
