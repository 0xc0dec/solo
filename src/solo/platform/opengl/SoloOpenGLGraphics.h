#pragma once

#include "SoloGraphics.h"

namespace solo
{
    class OpenGLRenderer;
    class Mesh;

    class OpenGLGraphics final: public Graphics
    {
    public:
        OpenGLGraphics(Device* device, const DeviceToken& token);

        void blit(Material* material, FrameBuffer* target) override final;

    private:
        Device* device = nullptr;
        OpenGLRenderer* renderer = nullptr;
        sptr<Mesh> quadMesh;
    };
}