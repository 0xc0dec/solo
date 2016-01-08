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
        virtual void renderSurface(Material* material, RenderTarget* target) override;

    private:
        friend class SurfaceRenderer;

        OpenGLSurfaceRenderer(Device *device, Material *material);

        Device *device;
        shared<Mesh> quadMesh;
        shared<MeshEffectBinding> binding;
    };
}