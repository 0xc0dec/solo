#pragma once

#include "SoloBase.h"


namespace solo
{
    class Renderer;
    class Device;
    class Mesh;
    class Material;
    class FrameBuffer;

    class Graphics
    {
    public:
        explicit Graphics(Device* device);

        SL_NONCOPYABLE(Graphics)

        void blit(Material* material, FrameBuffer* target);

    private:
        Device* device;
        Renderer* renderer;
        shared<Mesh> quadMesh;
    };
}