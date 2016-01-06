#pragma once

#include "SoloResourceManager.h"

namespace solo
{
    class StubResourceManager: public ResourceManager
    {
        friend class ResourceManager;

        explicit StubResourceManager(Device *device);
    };
}