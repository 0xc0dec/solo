#include "SoloSkyboxRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloResourceManager.h"
#include "SoloRenderContext.h"
#include "SoloCubeTexture.h"

using namespace solo;


SkyboxRenderer::SkyboxRenderer(Node node):
    ComponentBase(node)
{
    renderQueue = KnownRenderQueues::Skybox;

    auto device = node.getScene()->getDevice();
    auto resourceManager = device->getResourceManager();

    auto effect = resourceManager->getOrCreatePrefabEffect(EffectPrefab::Skybox, "/solo/internal/skybox-renderer/effect");
    material = resourceManager->getOrCreateMaterial(effect);
    material->setParameterAutoBinding("projMatrix", AutoBinding::ProjectionMatrix);
    material->setParameterAutoBinding("worldViewMatrix", AutoBinding::WorldViewMatrix);
    material->setDepthTest(true);
    material->setDepthWrite(false);
    material->setPolygonFace(PolygonFace::CW);

    quadMesh = resourceManager->getOrCreatePrefabMesh(MeshPrefab::Quad, "/solo/internal/skybox-renderer/mesh");
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
