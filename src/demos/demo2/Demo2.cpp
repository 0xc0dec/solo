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

#include "../../../include/Solo.h"
#include "../common/Screenshoter.h"
#include "../common/Rotator.h"
#include "../common/Shaders.h"
#include "Shaders.h"

using namespace solo;


class PostProcessor1 final
{
public:
    PostProcessor1(Device *device, Camera *camera):
        graphics(device->getGraphics()),
        camera(camera)
    {
        auto canvasSize = device->getCanvasSize();

        fbTex = RectTexture::create(device);
        fbTex->setData(TextureFormat::RGB, {}, static_cast<uint32_t>(canvasSize.x), static_cast<uint32_t>(canvasSize.y));
        fbTex->setFiltering(TextureFiltering::Nearest);
        fbTex->setWrapping(TextureWrapping::Clamp);
        fb1 = FrameBuffer::create(device);
        fb1->setAttachments({ fbTex });
        camera->setRenderTarget(fb1);

        fbTex2 = RectTexture::create(device);
        fbTex2->setData(TextureFormat::RGB, {}, static_cast<uint32_t>(canvasSize.x), static_cast<uint32_t>(canvasSize.y));
        fbTex2->setFiltering(TextureFiltering::Nearest);
        fbTex2->setWrapping(TextureWrapping::Clamp);
        fb2 = FrameBuffer::create(device);
        fb2->setAttachments({ fbTex2 });

        auto grayscaleEffect = Effect::create(device, commonShaders.vertex.passThrough, fsGrayscale);
        grayscaleMat = Material::create(device, grayscaleEffect);
        grayscaleMat->setFloatParameter("rightSeparator", 0.25f);

        auto saturateEffect = Effect::create(device, commonShaders.vertex.passThrough, fsSaturate);
        saturateMat = Material::create(device, saturateEffect);
        saturateMat->setFloatParameter("leftSeparator", 0.75f);
        saturateMat->setFloatParameter("rightSeparator", 1.0f);

        auto verticalBlurEffect = Effect::create(device, commonShaders.vertex.passThrough, fsVerticalBlur);
        verticalBlurMat = Material::create(device, verticalBlurEffect);
        verticalBlurMat->setFloatParameter("leftSeparator", 0.25f);
        verticalBlurMat->setFloatParameter("rightSeparator", 0.75f);

        auto horizontalBlurEffect = Effect::create(device, commonShaders.vertex.passThrough, fsHorizontalBlur);
        horizontalBlurMat = Material::create(device, horizontalBlurEffect);
        horizontalBlurMat->setFloatParameter("leftSeparator", 0.25f);
        horizontalBlurMat->setFloatParameter("rightSeparator", 0.75f);
    }

    ~PostProcessor1()
    {
        camera->setRenderTarget(nullptr);
    }

    void apply() const
    {
        // bounce between the two frame buffers
        grayscaleMat->setTextureParameter("mainTex", fbTex);
        graphics->blit(grayscaleMat.get(), fb2.get());

        saturateMat->setTextureParameter("mainTex", fbTex2);
        graphics->blit(saturateMat.get(), fb1.get());

        verticalBlurMat->setTextureParameter("mainTex", fbTex);
        graphics->blit(verticalBlurMat.get(), fb2.get());

        horizontalBlurMat->setTextureParameter("mainTex", fbTex2);
        graphics->blit(horizontalBlurMat.get(), nullptr);
    }

private:
    Graphics *graphics = nullptr;
    Camera *camera = nullptr;
    sptr<FrameBuffer> fb1 = nullptr;
    sptr<FrameBuffer> fb2 = nullptr;
    sptr<RectTexture> fbTex = nullptr;
    sptr<RectTexture> fbTex2 = nullptr;
    sptr<Material> grayscaleMat = nullptr;
    sptr<Material> saturateMat = nullptr;
    sptr<Material> verticalBlurMat = nullptr;
    sptr<Material> horizontalBlurMat = nullptr;
};


class PostProcessor2 final
{
public:
    PostProcessor2(Device *device, Camera *camera):
        device(device),
        loader(device->getAssetLoader()),
        graphics(device->getGraphics()),
        camera(camera)
    {
        const float stitchWidth = 30;

        stitchTex = loader->loadRectTexture("../assets/stitches.png");
        stitchTex->setFiltering(TextureFiltering::Nearest);

        auto canvasSize = device->getCanvasSize();
        auto stitchTexSize = stitchTex->getSize();

        auto resolution = Vector2(
            math::clamp(static_cast<int>(canvasSize.x / stitchWidth) * 2, 1, 2048),
            math::clamp(static_cast<int>(canvasSize.y / stitchTexSize.y) * 2, 1, 2048));

        auto stitchCount = Vector2(
            resolution.x * stitchWidth / (2 * stitchTexSize.x),
            resolution.y / 2);

        fbTex = RectTexture::create(device);
        fbTex->setData(TextureFormat::RGB, {}, static_cast<uint32_t>(resolution.x), static_cast<uint32_t>(resolution.y));
        fbTex->setFiltering(TextureFiltering::Nearest);
        fbTex->setWrapping(TextureWrapping::Clamp);
        fb1 = FrameBuffer::create(device);
        fb1->setAttachments({ fbTex });
        camera->setViewport(0, 0, resolution.x, resolution.y);
        camera->setRenderTarget(fb1);

        auto effect = Effect::create(device, commonShaders.vertex.passThrough, fsStitches);
        material = Material::create(device, effect);
        material->setTextureParameter("mainTex", fbTex);
        material->setTextureParameter("stitchTex", stitchTex);
        material->setVector2Parameter("stitchCount", stitchCount);
        material->setVector2Parameter("resolution", resolution);
    }

    ~PostProcessor2()
    {
        auto canvasSize = device->getCanvasSize();
        camera->setRenderTarget(nullptr);
        camera->setViewport(0, 0, canvasSize.x, canvasSize.y);
    }
    
    void apply() const
    {
        graphics->blit(material.get(), nullptr);
    }

private:
    Device *device = nullptr;
    AssetLoader *loader = nullptr;
    Graphics *graphics = nullptr;
    Camera *camera = nullptr;
    sptr<RectTexture> stitchTex;
    sptr<FrameBuffer> fb1;
    sptr<RectTexture> fbTex;
    sptr<Material> material;
};


class Demo final
{
public:
    explicit Demo(Device *device):
        device(device),
        scene(device->getScene()),
        loader(device->getAssetLoader())
    {
        initCamera();
        initSkybox();
        initMesh();
        device->getLogger()->logInfo("Press keys 1..3 to switch between modes");
    }

    void update()
    {
        scene->visit([](Component *cmp) { cmp->update(); });
        switchPostProcessors();
    }

    void render()
    {
        camera->apply([&](const RenderContext &ctx)
        {
            renderByTags(skyboxTag, ctx);
            renderByTags(~skyboxTag, ctx);
        });

        if (pp1)
            pp1->apply();
        else if (pp2)
            pp2->apply();
    }

private:
    void switchPostProcessors()
    {
        if (device->isKeyPressed(KeyCode::Digit1, true))
        {
            if (!pp1)
            {
                pp2 = nullptr;
                pp1 = std::make_unique<PostProcessor1>(device, camera);
            }
        }
        if (device->isKeyPressed(KeyCode::Digit2, true))
        {
            if (!pp2)
            {
                pp1 = nullptr;
                pp2 = std::make_unique<PostProcessor2>(device, camera);
            }
        }
        if (device->isKeyPressed(KeyCode::Digit3, true))
        {
            pp1 = nullptr;
            pp2 = nullptr;
        }
    }

    void renderByTags(uint32_t tags, const RenderContext &ctx)
    {
        scene->visit(tags, [=](Component *cmp) { cmp->render(ctx); });
    }

    void initCamera()
    {
        auto node = scene->createNode();
        
        camera = node->addComponent<Camera>();
        camera->setClearColor(0.0f, 0.6f, 0.6f, 1.0f);
        camera->setNear(0.05f);
        
        auto t = node->findComponent<Transform>();
        t->setLocalPosition(Vector3(0, 0, 5));
        
        node->addComponent<Screenshoter>("demo2-screenshot.bmp");
        
        auto spectator = node->addComponent<Spectator>();
        spectator->setVerticalRotationSpeed(1);
        spectator->setHorizontalRotationSpeed(1);

        pp1 = std::make_unique<PostProcessor1>(device, camera);
    }

    void initSkybox()
    {
        loader->loadCubeTextureAsync(
        {
            "../assets/skyboxes/deep-space/front.png",
            "../assets/skyboxes/deep-space/back.png",
            "../assets/skyboxes/deep-space/left.png",
            "../assets/skyboxes/deep-space/right.png",
            "../assets/skyboxes/deep-space/top.png",
            "../assets/skyboxes/deep-space/bottom.png"
        })->done([=](sptr<CubeTexture> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);
            tex->setFiltering(TextureFiltering::Linear);
            auto node = scene->createNode();
            auto renderer = node->addComponent<SkyboxRenderer>();
            renderer->setTexture(tex);
            renderer->setTags(skyboxTag);
        });
    }

    void initMesh()
    {
        loader->loadRectTextureAsync("../assets/cobblestone.png")->done([=](sptr<RectTexture> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);
            tex->generateMipmaps();
            auto effect = Effect::create(device, commonShaders.vertex.basic, commonShaders.fragment.texture);
            auto mat = Material::create(device, effect);
            mat->setFaceCull(FaceCull::All);
            mat->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
            mat->setTextureParameter("mainTex", tex);

            loader->loadMeshAsync("../assets/monkey.obj")->done([=](sptr<Mesh> mesh)
            {
                auto node = scene->createNode();
                auto renderer = node->addComponent<MeshRenderer>();
                renderer->setMesh(mesh);
                renderer->setMaterial(0, mat);
                node->findComponent<Transform>()->setLocalPosition(Vector3::zero());
                node->addComponent<Rotator>("local", Vector3::unitX());
            });
        });
    }

    const uint32_t skyboxTag = 1 << 1;

    uptr<PostProcessor1> pp1;
    uptr<PostProcessor2> pp2;
    Device *device = nullptr;
    Scene *scene = nullptr;
    AssetLoader *loader = nullptr;
    Camera *camera = nullptr;
};


int main()
{
    auto device = Device::create(DeviceSetup().withMode(DeviceMode::OpenGL).withDimensions(1200, 600).withLogFilePath("demo2.log"));
    Demo demo(device.get());
    while (!device->isQuitRequested() && !device->isWindowCloseRequested() && !device->isKeyPressed(KeyCode::Escape, true))
    {
        device->beginUpdate();
        device->getAssetLoader()->update();
        device->getRenderer()->beginFrame();

        demo.update();
        demo.render();

        device->getRenderer()->endFrame();
        device->endUpdate();
    }
    return 0;
}
