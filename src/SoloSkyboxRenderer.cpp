#include "SoloSkyboxRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloEffect.h"
#include "SoloRenderContext.h"
#include "SoloCubeTexture.h"

using namespace solo;


SkyboxRenderer::SkyboxRenderer(Node node):
    ComponentBase(node)
{
    renderQueue = KnownRenderQueues::Skybox;

    auto effect = Effect::create(EffectPrefab::Skybox);
    material = Material::create(effect);
    material->setParameterAutoBinding("projMatrix", AutoBinding::ProjectionMatrix);
    material->setParameterAutoBinding("worldViewMatrix", AutoBinding::WorldViewMatrix);
    material->setDepthTest(true);
    material->setDepthWrite(false);
    material->setPolygonFace(PolygonFace::CW);

    quadMesh = Mesh::create(MeshPrefab::Quad);
}


void SkyboxRenderer::render(RenderContext& context)
{
    material->bind(context);
    quadMesh->draw(material->getEffect());
}


void SkyboxRenderer::setTexture(sptr<CubeTexture> texture)
{
    this->texture = texture;
    material->setTextureParameter("mainTex", texture);
}
