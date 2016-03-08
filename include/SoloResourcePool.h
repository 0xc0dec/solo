#pragma once

#include "SoloBase.h"


namespace solo
{
    template <class T, int capacity>
    class ResourcePool final
    {
    public:
        ResourcePool()
        {
            for (auto i = 0; i < capacity; ++i)
                handles[i] = i;
        }

        int reserveHandle()
        {
            SL_DEBUG_THROW_IF(end >= capacity, InternalException, "Resource pool is full")
            return handles[end++];
        }

        void releaseHandle(int handle)
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

        int getHandleCount() const
        {
            return end;
        }

        int getHandle(int handleIndex) const
        {
            return handles[handleIndex];
        }

        T& getData(int handle)
        {
            return data[handle];
        }

    private:
        T data[capacity];
        int handles[capacity];
        int end = 0;
    };
}