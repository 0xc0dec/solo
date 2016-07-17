#pragma once

#include "SoloCommon.h"
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
        explicit SkyboxRenderer(const Node& node);

        void render(RenderContext& context) override final;

        void setTexture(sptr<CubeTexture> texture);
        auto getTexture() const -> sptr<CubeTexture>;

    private:
        sptr<Mesh> quadMesh;
        sptr<Material> material;
        sptr<CubeTexture> texture;
    };

    inline auto SkyboxRenderer::getTexture() const -> sptr<CubeTexture>
    {
        return texture;
    }
}