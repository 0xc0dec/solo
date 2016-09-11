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