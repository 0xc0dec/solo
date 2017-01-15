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

#include "../../../include/Solo.h"
#include "../common/Shaders.h"
#include "Shaders.h"

using namespace solo;


class PostProcessor1 final
{
public:
    PostProcessor1(Device *device, Camera *camera, uint32_t tag):
        mainCamera(camera)
    {
        auto canvasSize = device->getCanvasSize();

        fbTex1 = RectTexture::create(device);
        fbTex1->setData(TextureFormat::RGB, {}, static_cast<uint32_t>(canvasSize.x), static_cast<uint32_t>(canvasSize.y));
        fbTex1->setFiltering(TextureFiltering::Nearest);
        fbTex1->setWrapping(TextureWrapping::Clamp);
        fb1 = FrameBuffer::create(device);
        fb1->setAttachments({fbTex1});

        fbTex2 = RectTexture::create(device);
        fbTex2->setData(TextureFormat::RGB, {}, static_cast<uint32_t>(canvasSize.x), static_cast<uint32_t>(canvasSize.y));
        fbTex2->setFiltering(TextureFiltering::Nearest);
        fbTex2->setWrapping(TextureWrapping::Clamp);
        fb2 = FrameBuffer::create(device);
        fb2->setAttachments({fbTex2});

        auto grayscaleEffect = Effect::create(device, commonShaders.vertex.passThrough, fsGrayscale);
        grayscaleMat = Material::create(device, grayscaleEffect);
        grayscaleMat->setDepthTest(false);
        grayscaleMat->setDepthWrite(false);
        grayscaleMat->setFaceCull(FaceCull::All);
        grayscaleMat->setFloatParameter("rightSeparator", 0.25f);

        auto saturateEffect = Effect::create(device, commonShaders.vertex.passThrough, fsSaturate);
        saturateMat = Material::create(device, saturateEffect);
        saturateMat->setDepthTest(false);
        saturateMat->setDepthWrite(false);
        saturateMat->setFaceCull(FaceCull::All);
        saturateMat->setFloatParameter("leftSeparator", 0.75f);
        saturateMat->setFloatParameter("rightSeparator", 1.0f);

        auto verticalBlurEffect = Effect::create(device, commonShaders.vertex.passThrough, fsVerticalBlur);
        verticalBlurMat = Material::create(device, verticalBlurEffect);
        verticalBlurMat->setDepthTest(false);
        verticalBlurMat->setDepthWrite(false);
        verticalBlurMat->setFaceCull(FaceCull::All);
        verticalBlurMat->setFloatParameter("leftSeparator", 0.25f);
        verticalBlurMat->setFloatParameter("rightSeparator", 0.75f);

        auto horizontalBlurEffect = Effect::create(device, commonShaders.vertex.passThrough, fsHorizontalBlur);
        horizontalBlurMat = Material::create(device, horizontalBlurEffect);
        horizontalBlurMat->setDepthTest(false);
        horizontalBlurMat->setDepthWrite(false);
        horizontalBlurMat->setFaceCull(FaceCull::All);
        horizontalBlurMat->setFloatParameter("leftSeparator", 0.25f);
        horizontalBlurMat->setFloatParameter("rightSeparator", 0.75f);

        quadRenderer = camera->getNode().addComponent<MeshRenderer>();
        quadRenderer->setTags(tag);
        quadRenderer->setMesh(Mesh::create(device, MeshPrefab::Quad));

        mainCamera->setRenderTarget(fb1);
    }

    ~PostProcessor1()
    {
        mainCamera->getNode().removeComponent<MeshRenderer>();
        mainCamera->setRenderTarget(nullptr);
    }

    void apply() const
    {
        renderStep(grayscaleMat, fbTex1, fb2);
        renderStep(saturateMat, fbTex2, fb1);
        renderStep(verticalBlurMat , fbTex1, fb2);
        renderStep(horizontalBlurMat, fbTex2, nullptr);

        mainCamera->setRenderTarget(fb1);
    }

private:
    void renderStep(sptr<Material> mat, sptr<Texture> inputTexture, sptr<FrameBuffer> target) const
    {
        mat->setTextureParameter("mainTex", inputTexture);
        quadRenderer->setMaterial(0, mat);
        mainCamera->setRenderTarget(target);
        mainCamera->apply([=](const RenderContext &ctx) { quadRenderer->render(ctx);});
    }

    MeshRenderer *quadRenderer = nullptr;
    Camera *mainCamera = nullptr;
    sptr<FrameBuffer> fb1 = nullptr;
    sptr<FrameBuffer> fb2 = nullptr;
    sptr<RectTexture> fbTex1 = nullptr;
    sptr<RectTexture> fbTex2 = nullptr;
    sptr<Material> grayscaleMat = nullptr;
    sptr<Material> saturateMat = nullptr;
    sptr<Material> verticalBlurMat = nullptr;
    sptr<Material> horizontalBlurMat = nullptr;
};
