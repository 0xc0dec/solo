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

#include "../common/DemoBase.h"
#include "../common/Screenshoter.h"
#include "../common/Rotator.h"
#include "../common/Shaders.h"
#include "PostProcessor1.h"
#include "PostProcessor2.h"

using namespace solo;


class Demo final: public DemoBase
{
public:
    explicit Demo(Device *device): DemoBase(device)
    {
        initCamera();
        initSkybox();
        initMesh();
        device->getLogger()->logInfo("Press keys 1..3 to switch between modes");
    }

private:
    void update() override final
    {
        DemoBase::update();
        switchPostProcessors();
    }

    void render() override final
    {
        camera->render([&](const RenderContext &ctx)
        {
            renderByTags(skyboxTag, ctx);
            renderByTags(~(skyboxTag | postProcessorTag), ctx);
        });

        if (pp1)
            pp1->apply();
        else if (pp2)
            pp2->apply();
    }

    void switchPostProcessors()
    {
        if (device->isKeyPressed(KeyCode::Digit1, true))
        {
            if (!pp1)
            {
                pp2 = nullptr;
                pp1 = std::make_unique<PostProcessor1>(device, camera, postProcessorTag);
            }
        }
        if (device->isKeyPressed(KeyCode::Digit2, true))
        {
            if (!pp2)
            {
                pp1 = nullptr;
                pp2 = std::make_unique<PostProcessor2>(device, camera, postProcessorTag);
            }
        }
        if (device->isKeyPressed(KeyCode::Digit3, true))
        {
            pp1 = nullptr;
            pp2 = nullptr;
        }
    }

    void initCamera()
    {
        auto node = scene->createNode();
        
        camera = node->addComponent<Camera>();
        camera->setClearColor(0.0f, 0.6f, 0.6f, 1.0f);
        camera->setNear(0.05f);
        
        node->findComponent<Transform>()->setLocalPosition(Vector3(0, 0, 5));
        
        node->addComponent<Screenshoter>("demo2-screenshot.bmp");
        node->addComponent<Spectator>();

        pp1 = std::make_unique<PostProcessor1>(device, camera, postProcessorTag);
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
    const uint32_t postProcessorTag = 1 << 2;

    uptr<PostProcessor1> pp1;
    uptr<PostProcessor2> pp2;
    Camera *camera = nullptr;
};


int main()
{
    auto device = Device::create(DeviceSetup()
        .withMode(DeviceMode::OpenGL)
        .withDimensions(1200, 600)
        .withLogFilePath("demo2.log")
        .withWindowTitle("Demo 2"));
    Demo(device.get()).run();
    return 0;
}
