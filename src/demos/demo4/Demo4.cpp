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
#include "../common/Shaders.h"

using namespace solo;


class Demo final: public DemoBase
{
public:
    explicit Demo(Device* device): DemoBase(device)
    {
        initObjects();
        initCamera();
        initSkybox();
    }

private:
    void render() override final
    {
        camera->render([&](const RenderContext& ctx)
        {
            renderByTags(skyboxTag, ctx);
            renderByTags(~skyboxTag, ctx);
        });
    }

    void initCamera()
    {
        auto node = scene->createNode();
        auto t = node->findComponent<Transform>();
        t->setLocalPosition({10, 10, 10});
        t->lookAt({}, Vector3::unitY());

        camera = node->addComponent<Camera>();
        camera->setClearColor(0.0f, 0.6f, 0.6f, 1.0f);
        camera->setNear(0.05f);

        node->addComponent<Screenshoter>("demo3-screenshot.bmp");
        node->addComponent<Spawner>(cubeMesh);
        node->addComponent<Targeter>();

        node->addComponent<Spectator>();
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

    void initObjects()
    {
        auto tex = loader->loadRectTexture("../assets/cobblestone.png");
        tex->setWrapping(TextureWrapping::Clamp);
        tex->generateMipmaps();

        auto mat = Material::create(device, Effect::create(device, commonShaders.vertex.basic, commonShaders.fragment.texture));
        mat->setFaceCull(FaceCull::All);
        mat->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        mat->setTextureParameter("mainTex", tex);

        cubeMesh = Mesh::create(device, MeshPrefab::Cube);

        // Floor
        auto node = scene->createNode();
        node->findComponent<Transform>()->setLocalScale({10, 0.1f, 10});
        auto renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(cubeMesh);
        renderer->setMaterial(0, mat);

        auto rigidBody = node->addComponent<RigidBody>(RigidBodyConstructionParameters().withMass(0).withFriction(0.5f));
        rigidBody->setCollider(BoxCollider::create(Vector3::unit()));
    }

    const uint32_t skyboxTag = 1 << 2;

    Camera *camera = nullptr;
    sptr<Mesh> cubeMesh;
};


int main()
{
    auto device = Device::create(DeviceSetup()
        .withMode(DeviceMode::OpenGL)
        .withDimensions(1200, 600)
        .withLogFilePath("demo4.log")
        .withWindowTitle("Demo 4"));
    Demo(device.get()).run();
    return 0;
}
