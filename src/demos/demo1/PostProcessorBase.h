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

#pragma once

#include <Solo.h>

using namespace solo;


class PostProcessorBase
{
public:
    PostProcessorBase(Device* device, Camera* camera, uint32_t tag):
        device(device),
        loader(device->getAssetLoader()),
        camera(camera)
    {
        quadRenderer = camera->getNode().addComponent<MeshRenderer>();
        quadRenderer->setTags(tag);
        quadRenderer->setMesh(Mesh::createFromPrefab(device, MeshPrefab::Quad));
    }

    ~PostProcessorBase()
    {
        quadRenderer->getNode().removeComponent<MeshRenderer>();
    }

protected:
    void renderStep(sptr<Material> mat, sptr<Texture> inputTexture, sptr<FrameBuffer> target, Vector4 viewport) const
    {
        mat->setTextureParameter("mainTex", inputTexture);
        quadRenderer->setMaterial(0, mat);
        camera->setViewport({viewport.x, viewport.y, viewport.z, viewport.w});
        camera->setRenderTarget(target);
        camera->render([=](const RenderContext &ctx) { quadRenderer->render(ctx);});
    }

    MeshRenderer *quadRenderer = nullptr;
    Device *device = nullptr;
    AssetLoader *loader = nullptr;
    Camera *camera = nullptr;
};