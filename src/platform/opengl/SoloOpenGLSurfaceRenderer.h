#pragma once

#include "SoloSurfaceRenderer.h"

namespace solo
{
    class Device;
    class Material;
    class MeshEffectBinding;
    class Mesh;

    class OpenGLSurfaceRenderer: public SurfaceRenderer
    {
    public:
        virtual void renderSurface(RenderTarget* target) override;

    private:
        friend class SurfaceRenderer;

        OpenGLSurfaceRenderer(Device* device, shared<Material> material);

        Device* device;
        shared<Mesh> mesh;
        shared<Material> material;
        shared<MeshEffectBinding> binding;
    };
}