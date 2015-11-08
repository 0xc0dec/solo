#include "SoloSkyboxRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloMaterialParameter.h"
#include "SoloDevice.h"
#include "SoloResourceManager.h"
#include "SoloRenderContext.h"
#include "SoloCubeTexture.h"

using namespace solo;


shared<SkyboxRenderer> SkyboxRenderer::create(Node node)
{
	return SL_NEW2(SkyboxRenderer, node);
}


SkyboxRenderer::SkyboxRenderer(Node node):
	ComponentBase(node)
{
	auto resourceManager = node.getScene()->getDevice()->getResourceManager();
	quadMesh = resourceManager->getOrCreateMesh(KnownUris::UnitQuadMesh);
	auto effect = resourceManager->findEffect(KnownUris::SkyboxEffect);
	material = resourceManager->getOrCreateMaterial(effect); // TODO use a known uri?
	material->getParameter("projMatrix")->bindValue(AutoBinding::ProjectionMatrix);
	material->getParameter("worldViewMatrix")->bindValue(AutoBinding::WorldViewMatrix);
//	material->setDepthTestEnabled(false);
//	material->setDepthWriteEnabled(false);
	material->setPolygonFace(PolygonFace::All);
}


void SkyboxRenderer::render(RenderContext& context)
{
	material->bind(context);
	quadMesh->draw();
	material->unbind(context);
}


void SkyboxRenderer::setTexture(shared<CubeTexture> texture)
{
	this->texture = texture;
	material->getParameter("mainTex")->setTexture(texture);
}
