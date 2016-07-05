#include "SoloSkyboxRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloRenderContext.h"
#include "SoloCubeTexture.h"

using namespace solo;


SkyboxRenderer::SkyboxRenderer(const Node& node):
    ComponentBase(node)
{
    renderQueue = KnownRenderQueues::Skybox;

    auto effect = Effect::getPrefabInstance(EffectPrefab::Skybox);
    material = Material::create(effect);
    material->bindProjectionMatrixParameter("projMatrix");
    material->bindWorldViewMatrixParameter("worldViewMatrix");
    material->setDepthTest(true);
    material->setDepthWrite(false);
    material->setFaceCull(FaceCull::CW);

    quadMesh = Mesh::create(MeshPrefab::Quad);
}


void SkyboxRenderer::render(RenderContext& context)
{
    material->apply(context);
    quadMesh->draw(material->getEffect());
}


void SkyboxRenderer::setTexture(sptr<CubeTexture> texture)
{
    this->texture = texture;
    material->setTextureParameter("mainTex", texture);
}
