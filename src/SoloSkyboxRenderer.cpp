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
    renderQueue = KnownRenderQueues::Skyboxes;

    auto device = node.getScene()->getDevice();
    auto resourceManager = device->getResourceManager();

    auto effect = resourceManager->getOrCreatePrefabEffect(EffectPrefab::Skybox, "/solo/internal/skybox-renderer/effect");
    material = resourceManager->getOrCreateMaterial(effect);
    material->setParameterAutoBinding("projMatrix", AutoBinding::ProjectionMatrix);
    material->setParameterAutoBinding("worldViewMatrix", AutoBinding::WorldViewMatrix);
    material->setDepthTestEnabled(true);
    material->setDepthWriteEnabled(false);
    material->setPolygonFace(PolygonFace::CW);

    quadMesh = resourceManager->getOrCreatePrefabMesh(MeshPrefab::Quad, "/solo/internal/skybox-renderer/mesh");
}


void SkyboxRenderer::render(RenderContext& context)
{
    material->bind(context);
    quadMesh->drawIndex(0, material->getEffect());
    material->unbind(context);
}


void SkyboxRenderer::setTexture(shared<CubeTexture> texture)
{
    this->texture = texture;
    material->setTextureParameter("mainTex", texture);
}
