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

        void setTexture(sptr<CubeTexture> texture);
        sptr<CubeTexture> getTexture() const;

    private:
        sptr<Mesh> quadMesh;
        sptr<Material> material;
        sptr<CubeTexture> texture;
    };

    inline sptr<CubeTexture> SkyboxRenderer::getTexture() const
    {
        return texture;
    }
}