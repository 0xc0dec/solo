#pragma once

#include "SoloResourceManager.h"

namespace solo
{
    class StubResourceManager final: public ResourceManager
    {
        friend class ResourceManager;

        explicit StubResourceManager(Device* device);
    };
}