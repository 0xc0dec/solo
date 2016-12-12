/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloSkyboxRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloRenderContext.h"
#include "SoloCubeTexture.h"

using namespace solo;


SkyboxRenderer::SkyboxRenderer(const Node &node):
    ComponentBase(node)
{
    renderQueue = KnownRenderQueues::Skybox;

    auto effect = Effect::create(node.getScene()->getDevice(), EffectPrefab::Skybox);
    material = Material::create(node.getScene()->getDevice(), effect);
    material->bindProjectionMatrixParameter("projMatrix");
    material->bindWorldViewMatrixParameter("worldViewMatrix");
    material->setDepthTest(true);
    material->setDepthWrite(false);
    material->setFaceCull(FaceCull::CW);

    quadMesh = Mesh::create(MeshPrefab::Quad);
}


void SkyboxRenderer::render(RenderContext &context)
{
    material->apply(context);
    quadMesh->draw(material->getEffect());
}


void SkyboxRenderer::setTexture(sptr<CubeTexture> texture)
{
    this->texture = texture;
    material->setTextureParameter("mainTex", texture);
}
