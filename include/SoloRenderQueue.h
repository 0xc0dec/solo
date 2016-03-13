#pragma once

namespace solo
{
    struct KnownRenderQueues
    {
        static constexpr uint32_t NotRendered = 0;
        static constexpr uint32_t CameraDefault = 100;
        static constexpr uint32_t Skyboxes = 500;
        static constexpr uint32_t OpaqueObjects = 1000;
    };
}
