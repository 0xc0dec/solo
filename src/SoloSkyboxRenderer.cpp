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


shared<SkyboxRenderer> SkyboxRenderer::create(Node node)
{
    return SL_NEW_SHARED(SkyboxRenderer, node);
}


SkyboxRenderer::SkyboxRenderer(Node node):
    ComponentBase(node)
{
    renderQueue = KnownRenderQueues::Skyboxes;

    auto resourceManager = node.getScene()->getDevice()->getResourceManager();

    quadMesh = resourceManager->getOrCreatePrefabMesh(MeshPrefab::Quad, "/solo/internal/skybox-renderer/mesh");

    auto effect = resourceManager->getOrCreatePrefabEffect(EffectPrefab::Skybox, "/solo/internal/skybox-renderer/effect");
    material = resourceManager->getOrCreateMaterial(effect);
    material->getParameter("projMatrix")->bindValue(AutoBinding::ProjectionMatrix);
    material->getParameter("worldViewMatrix")->bindValue(AutoBinding::WorldViewMatrix);
    material->setDepthTestEnabled(true);
    material->setDepthWriteEnabled(false);
    material->setPolygonFace(PolygonFace::CW);
    
    binding = MeshEffectBinding::create(node.getScene()->getDevice()->getMode(), quadMesh.get(), material->getEffect());
}


void SkyboxRenderer::render(RenderContext &context)
{
    material->bind(context);
    binding->bind();
    quadMesh->drawPart(0);
    binding->unbind();
    material->unbind(context);
}


void SkyboxRenderer::setTexture(shared<CubeTexture> texture)
{
    this->texture = texture;
    material->getParameter("mainTex")->setTexture(texture);
}
