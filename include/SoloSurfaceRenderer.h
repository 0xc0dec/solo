#pragma once

#include "SoloBase.h"
#include "SoloMesh.h"


namespace solo
{
    class Material;
    class FrameBuffer;
    class Renderer;
    class Device;

    class SurfaceRenderer final
    {
    public:
        SurfaceRenderer(Renderer* renderer, shared<Material> material);
        ~SurfaceRenderer();

        SL_NONCOPYABLE(SurfaceRenderer)

        void renderSurface(FrameBuffer* target);

    private:
        Renderer* renderer;
        Device* device;
        shared<Material> material;
        shared<Mesh> mesh;
        MeshEffectBinding binding;
    };
}