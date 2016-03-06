#include "SoloSkyboxRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloEffect.h"
#include "SoloMaterialParameter.h"
#include "SoloDevice.h"
#include "SoloResourceManager.h"
#include "SoloRenderContext.h"
#include "SoloCubeTexture.h"
#include "SoloMeshEffectBinding.h"

using namespace solo;


SkyboxRenderer::SkyboxRenderer(Node node):
    ComponentBase(node)
{
    renderQueue = KnownRenderQueues::Skyboxes;

    auto device = node.getScene()->getDevice();
    auto resourceManager = device->getResourceManager();

    auto effect = resourceManager->getOrCreatePrefabEffect(EffectPrefab::Skybox, "/solo/internal/skybox-renderer/effect");
    material = resourceManager->getOrCreateMaterial(effect);
    material->getParameter("projMatrix")->bindValue(AutoBinding::ProjectionMatrix);
    material->getParameter("worldViewMatrix")->bindValue(AutoBinding::WorldViewMatrix);
    material->setDepthTestEnabled(true);
    material->setDepthWriteEnabled(false);
    material->setPolygonFace(PolygonFace::CW);

    quadMesh = resourceManager->getOrCreatePrefabMesh(MeshPrefab::Quad, "/solo/internal/skybox-renderer/mesh");

    binding = MeshEffectBinding::create(device->getMode(), quadMesh.get(), material->getEffect());
}


void SkyboxRenderer::render(RenderContext& context)
{
    material->bind(context);
    binding->bind();
    quadMesh->drawIndex(0);
    binding->unbind();
    material->unbind(context);
}


void SkyboxRenderer::setTexture(shared<CubeTexture> texture)
{
    this->texture = texture;
    material->getParameter("mainTex")->setTexture(texture);
}
