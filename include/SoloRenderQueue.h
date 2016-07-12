#pragma once

namespace solo
{
    struct KnownRenderQueues final
    {
        static constexpr uint32_t NotRendered = 0;
        static constexpr uint32_t Camera = 100;
        static constexpr uint32_t Skybox = 500;
        static constexpr uint32_t Opaque = 1000;
        static constexpr uint32_t Transparent = 2000;
    };
}
