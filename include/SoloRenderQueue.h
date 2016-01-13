#pragma once

namespace solo
{
    struct KnownRenderQueues
    {
        static constexpr int NotRendered = 0;
        static constexpr int CameraDefault = 100;
        static constexpr int Skyboxes = 500;
        static constexpr int OpaqueObjects = 1000;
    };
}
