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

#include "PostProcessorBase.h"
#include "Shaders.h"

using namespace solo;


class PostProcessor1 final: public PostProcessorBase
{
public:
    PostProcessor1(Device *device, Camera *camera, uint32_t tag):
        PostProcessorBase(device, camera, tag)
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

        auto grayscaleEffect = Effect::create(device, vsPassThrough, fsGrayscale);
        grayscaleMat = Material::create(device, grayscaleEffect);
        grayscaleMat->setDepthTest(false);
        grayscaleMat->setDepthWrite(false);
        grayscaleMat->setFaceCull(FaceCull::All);
        grayscaleMat->setFloatParameter("rightSeparator", 0.25f);

        auto saturateEffect = Effect::create(device, vsPassThrough, fsSaturate);
        saturateMat = Material::create(device, saturateEffect);
        saturateMat->setDepthTest(false);
        saturateMat->setDepthWrite(false);
        saturateMat->setFaceCull(FaceCull::All);
        saturateMat->setFloatParameter("leftSeparator", 0.75f);
        saturateMat->setFloatParameter("rightSeparator", 1.0f);

        auto verticalBlurEffect = Effect::create(device, vsPassThrough, fsVerticalBlur);
        verticalBlurMat = Material::create(device, verticalBlurEffect);
        verticalBlurMat->setDepthTest(false);
        verticalBlurMat->setDepthWrite(false);
        verticalBlurMat->setFaceCull(FaceCull::All);
        verticalBlurMat->setFloatParameter("leftSeparator", 0.25f);
        verticalBlurMat->setFloatParameter("rightSeparator", 0.75f);

        auto horizontalBlurEffect = Effect::create(device, vsPassThrough, fsHorizontalBlur);
        horizontalBlurMat = Material::create(device, horizontalBlurEffect);
        horizontalBlurMat->setDepthTest(false);
        horizontalBlurMat->setDepthWrite(false);
        horizontalBlurMat->setFaceCull(FaceCull::All);
        horizontalBlurMat->setFloatParameter("leftSeparator", 0.25f);
        horizontalBlurMat->setFloatParameter("rightSeparator", 0.75f);

        camera->setRenderTarget(fb1);
    }

    ~PostProcessor1()
    {
        camera->setRenderTarget(nullptr);
    }

    void apply() const
    {
        static const Vector4 viewport{-1, -1, -1, -1};

        renderStep(grayscaleMat, fbTex1, fb2, viewport);
        renderStep(saturateMat, fbTex2, fb1, viewport);
        renderStep(verticalBlurMat , fbTex1, fb2, viewport);
        renderStep(horizontalBlurMat, fbTex2, nullptr, viewport);

        camera->setRenderTarget(fb1);
    }

private:
    sptr<FrameBuffer> fb1 = nullptr;
    sptr<FrameBuffer> fb2 = nullptr;
    sptr<RectTexture> fbTex1 = nullptr;
    sptr<RectTexture> fbTex2 = nullptr;
    sptr<Material> grayscaleMat = nullptr;
    sptr<Material> saturateMat = nullptr;
    sptr<Material> verticalBlurMat = nullptr;
    sptr<Material> horizontalBlurMat = nullptr;
};
