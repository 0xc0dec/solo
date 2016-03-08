#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloNode.h"
#include "SoloMesh.h"


namespace solo
{
    class Material;
    class CubeTexture;

    class SkyboxRenderer final: public ComponentBase<SkyboxRenderer>
    {
    public:
        explicit SkyboxRenderer(Node node);

        virtual void render(RenderContext& context) override final;
        virtual void terminate() override final;

        void setTexture(shared<CubeTexture> texture);
        shared<CubeTexture> getTexture() const;

    private:
        shared<Mesh> quadMesh;
        shared<Material> material;
        shared<CubeTexture> texture;
        MeshEffectBinding binding;
    };

    inline shared<CubeTexture> SkyboxRenderer::getTexture() const
    {
        return texture;
    }
}