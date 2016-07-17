#pragma once

#include "SoloCommon.h"
#include <functional>


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

        auto reserveHandle()
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

        auto getHandleCount() const
        {
            return end;
        }

        auto getHandle(uint16_t handleIndex) const
        {
            return handles[handleIndex];
        }

        auto& getData(uint16_t handle)
        {
            return data[handle];
        }

        void cleanup(std::function<void(const T&)> deleteResource)
        {
            auto handle = getHandle(0);
            auto data = getData(handle);
            deleteResource(data);
            releaseHandle(handle);
        }

    private:
        T data[capacity];
        uint16_t handles[capacity];
        uint16_t end = 0;
    };
}