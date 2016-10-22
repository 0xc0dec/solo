#pragma once

#include "SoloGraphics.h"

namespace solo
{
    class NullGraphics final: public Graphics
    {
    public:
        explicit NullGraphics(const DeviceToken& token): Graphics(token)
        {
        }

        void blit(Material* material, FrameBuffer* target) override final
        {
        }
    };
}