#pragma once

#include "SoloBase.h"


namespace solo
{
    template <class T, uint16_t capacity>
    class ResourcePool final
    {
    public:
        ResourcePool()
        {
            for (auto i = 0; i < capacity; ++i)
                handles[i] = i;
        }

        uint16_t reserveHandle()
        {
            return handles[end++];
        }

        void releaseHandle(uint16_t handle)
        {
            for (auto i = 0; i < end; ++i)
            {
                if (handles[i] == handle)
                {
                    end--;
                    std::swap(handles[i], handles[end]);
                    return;
                }
            }
        }

        uint16_t getHandleCount() const
        {
            return end;
        }

        uint16_t getHandle(uint16_t handleIndex) const
        {
            return handles[handleIndex];
        }

        T& getData(uint16_t handle)
        {
            return data[handle];
        }

    private:
        T data[capacity];
        uint16_t handles[capacity];
        uint16_t end = 0;
    };
}