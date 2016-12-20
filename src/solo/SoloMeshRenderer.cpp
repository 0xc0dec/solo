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

#include "SoloMeshRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloRenderContext.h"
#include "SoloTransform.h"

using namespace solo;


MeshRenderer::MeshRenderer(const Node &node):
    ComponentBase(node)
{
    renderQueue = KnownRenderQueues::Opaque;
    transform = node.findComponent<Transform>();
}


void MeshRenderer::render(RenderContext &context)
{
    if (!mesh || materials.empty())
        return;

    auto partCount = mesh->getPartCount();
    if (partCount == 0)
    {
        auto material = getMaterial(0);
        if (material)
        {
            material->apply(context.camera, context.cameraTransform, transform);
            mesh->draw(material->getEffect());
        }
    }
    else
    {
        for (uint32_t part = 0; part < partCount; ++part)
        {
            auto material = getMaterial(part);
            if (material)
            {
                material->apply(context.camera, context.cameraTransform, transform);
                mesh->drawPart(material->getEffect(), part);
            }
        }
    }
}


void MeshRenderer::setMaterial(uint32_t index, sptr<Material> material)
{
    if (material)
        materials[index] = material;
    else
        materials.erase(index);
}
