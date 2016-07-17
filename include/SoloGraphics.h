#pragma once

#include "SoloCommon.h"


namespace solo
{
    class Renderer;
    class Mesh;
    class Material;
    class FrameBuffer;
    class DeviceToken;
    class Device;

    class Graphics
    {
    public:
        Graphics(Device* device, const DeviceToken&);
        SL_NONCOPYABLE(Graphics)

        void blit(Material* material, FrameBuffer* target);

    private:
        Device* device;
        Renderer* renderer;
        sptr<Mesh> quadMesh;
    };
}