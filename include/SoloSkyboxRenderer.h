/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloComponent.h"
#include "SoloNode.h"
#include "SoloMesh.h"

namespace solo
{
    class Material;
    class CubeTexture;
    class Transform;
    class Renderer;

    class SkyboxRenderer final: public ComponentBase<SkyboxRenderer>
    {
    public:
        explicit SkyboxRenderer(const Node &node);

        void render(const RenderContext &context) override final;

        void setTexture(sptr<CubeTexture> texture);
        auto getTexture() const -> sptr<CubeTexture>;

    private:
        Renderer *renderer = nullptr;
        Transform *transform = nullptr;
        sptr<Mesh> quadMesh;
        sptr<Material> material;
        sptr<CubeTexture> texture;
    };

    inline auto SkyboxRenderer::getTexture() const -> sptr<CubeTexture>
    {
        return texture;
    }
}