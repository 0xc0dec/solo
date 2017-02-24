/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSkyboxRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloRenderContext.h"
#include "SoloCubeTexture.h"
#include "SoloTransform.h"

using namespace solo;


SkyboxRenderer::SkyboxRenderer(const Node &node):
    ComponentBase(node)
{
    transform = node.findComponent<Transform>();

    auto device = node.getScene()->getDevice();

    auto effect = Effect::createFromPrefab(device, EffectPrefab::Skybox);
    material = Material::create(device, effect);
    material->bindProjectionMatrixParameter("projMatrix");
    material->bindWorldViewMatrixParameter("worldViewMatrix");
    material->setDepthTest(true);
    material->setDepthWrite(false);
    material->setFaceCull(FaceCull::CW);

    quadMesh = Mesh::createFromPrefab(device, MeshPrefab::Quad);
}


void SkyboxRenderer::render(const RenderContext &context)
{
    material->apply(context.camera, transform);
    quadMesh->draw(material->getEffect());
}


void SkyboxRenderer::setTexture(sptr<CubeTexture> texture)
{
    this->texture = texture;
    material->setTextureParameter("mainTex", texture);
}
