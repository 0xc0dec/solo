#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloNode.h"
#include "SoloRenderQueue.h"

namespace solo
{
    class Mesh2;
    class Material;
    class CubeTexture;

    class SkyboxRenderer: public ComponentBase<SkyboxRenderer>
    {
    public:
        static shared<SkyboxRenderer> create(Node node);

        virtual void render(RenderContext &context) override;

        void setTexture(shared<CubeTexture> texture);
        shared<CubeTexture> getTexture() const;

    private:
        explicit SkyboxRenderer(Node node);

        shared<Mesh2> quadMesh;
        shared<Material> material;
        shared<CubeTexture> texture;
    };

    inline shared<CubeTexture> SkyboxRenderer::getTexture() const
    {
        return texture;
    }
}