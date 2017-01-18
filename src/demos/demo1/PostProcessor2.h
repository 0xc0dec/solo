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


class PostProcessor2 final: public PostProcessorBase
{
public:
    PostProcessor2(Device *device, Camera *camera, uint32_t tag):
        PostProcessorBase(device, camera, tag)
    {
        const float stitchWidth = 30;

        canvasSize = device->getCanvasSize();

        stitchTex = loader->loadRectTexture("../assets/stitches.png");
        stitchTex->setFiltering(TextureFiltering::Nearest);

        auto stitchTexSize = stitchTex->getSize();

        offscreenRes = {
            math::clamp(static_cast<int>(canvasSize.x / stitchWidth) * 2, 1, 2048),
            math::clamp(static_cast<int>(canvasSize.y / stitchTexSize.y) * 2, 1, 2048)};

        auto stitchCount = Vector2(
            offscreenRes.x * stitchWidth / (2 * stitchTexSize.x),
            offscreenRes.y / 2);

        fbTex = RectTexture::create(device);
        fbTex->setData(TextureFormat::RGB, {}, static_cast<uint32_t>(offscreenRes.x), static_cast<uint32_t>(offscreenRes.y));
        fbTex->setFiltering(TextureFiltering::Nearest);
        fbTex->setWrapping(TextureWrapping::Clamp);
        fb1 = FrameBuffer::create(device);
        fb1->setAttachments({fbTex});

        auto effect = Effect::create(device, vsPassThrough, fsStitches);
        material = Material::create(device, effect);
        material->setTextureParameter("mainTex", fbTex);
        material->setTextureParameter("stitchTex", stitchTex);
        material->setVector2Parameter("stitchCount", stitchCount);
        material->setVector2Parameter("resolution", offscreenRes);

        camera->setViewport({0, 0, offscreenRes.x, offscreenRes.y});
        camera->setRenderTarget(fb1);
    }

    ~PostProcessor2()
    {
        camera->setRenderTarget(nullptr);
        camera->setViewport({0, 0, canvasSize.x, canvasSize.y});
    }
    
    void apply() const
    {
        renderStep(material, fbTex, nullptr, {0, 0, canvasSize.x, canvasSize.y});

        camera->setViewport({0, 0, offscreenRes.x, offscreenRes.y});
        camera->setRenderTarget(fb1);
    }

private:
    Vector2 offscreenRes;
    Vector2 canvasSize;
    sptr<RectTexture> stitchTex;
    sptr<FrameBuffer> fb1;
    sptr<RectTexture> fbTex;
    sptr<Material> material;
};
