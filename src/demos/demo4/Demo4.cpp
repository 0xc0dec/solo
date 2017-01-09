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
#include "../common/Shaders.h"

using namespace solo;


class SpawnedObject final: public ComponentBase<SpawnedObject>
{
public:
    explicit SpawnedObject(const Node &node, sptr<Effect> effect, sptr<Mesh> mesh,
        const Vector3 &initialPos, const Quaternion &initialRotation):
        ComponentBase<SpawnedObject>(node),
        device(node.getScene()->getDevice()),
        scene(node.getScene()),
        initialPos(initialPos),
        initialRotation(initialRotation),
        mesh(mesh)
    {
        material = Material::create(device, effect);
        material->setFaceCull(FaceCull::All);
        material->setPolygonMode(PolygonMode::Wireframe);
        material->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        material->setVector4Parameter("color", Vector4(1, 1, 0, 1));
    }

    void setActive()
    {
        setColor(Vector4(1, 0, 0, 1));
        active = true;
        activeTimer = 0;
    }

    void init() override final
    {
        auto renderer = node.addComponent<MeshRenderer>();
        renderer->setMesh(mesh);
        renderer->setMaterial(0, material);
        auto t = node.findComponent<Transform>();
        t->setLocalScale(Vector3(0.3f, 0.3f, 0.3f));
        t->setLocalPosition(initialPos);
        t->setLocalRotation(initialRotation);

        auto rigidBody = node.addComponent<RigidBody>(
                             RigidBodyConstructionParameters()
                             .withMass(50).withRestitution(0.5f).withFriction(0.2f)
                             .withLinearDamping(0.1f).withAngularDamping(0.1f)
                         );
        rigidBody->setCollider(BoxCollider::create(Vector3::unit()));
    }

    void update() override final
    {
        if (!active)
            return;
        activeTimer += device->getTimeDelta();
        if (activeTimer >= 0.2f)
        {
            setColor(Vector4(1, 1, 0, 1));
            active = false;
        }
    }

private:
    void setColor(const Vector4 &color)
    {
        material->setVector4Parameter("color", color);
    }

    Device *device = nullptr;
    Scene *scene = nullptr;
    Vector3 initialPos;
    Quaternion initialRotation;
    sptr<Material> material;
    sptr<Mesh> mesh;
    bool active = false;
    float activeTimer = 0;
};


class Targeter final: public ComponentBase<Targeter>
{
public:
    explicit Targeter(const Node &node):
        ComponentBase<Targeter>(node),
        physics(node.getScene()->getDevice()->getPhysics())
    {
    }

    void init() override final
    {
        transform = node.findComponent<Transform>();
    }

    void update() override final
    {
        auto hitResults = physics->castRayAll(
            transform->getWorldPosition(),
            transform->getWorldPosition() + transform->getLocalForward() * 100);

        for (const auto &result : hitResults)
        {
            auto obj = result.body->getNode().findComponent<SpawnedObject>();
            if (obj)
                obj->setActive();
        }
    }

private:
    Vector4 normalColor = Vector4(1, 1, 0, 1);
    Vector4 activeColor = Vector4(1, 0, 0, 1);
    Physics *physics = nullptr;
    Transform *transform = nullptr;
};


class Spawner final: public ComponentBase<Spawner>
{
public:
    explicit Spawner(const Node &node, sptr<Mesh> cubeMesh):
        ComponentBase<Spawner>(node),
        device(node.getScene()->getDevice()),
        scene(device->getScene()),
        transform(node.findComponent<Transform>()),
        mesh(cubeMesh)
    {
    }

    void init() override final
    {
        effect = Effect::create(device, commonShaders.vertex.basic, commonShaders.fragment.color);
    }

    void update() override final
    {
        if (device->isKeyPressed(KeyCode::Space, true))
            spawn();
    }

private:
    void spawn()
    {
        auto initialPos = transform->getLocalPosition() + transform->getLocalForward() * 2;
        auto initialRotation = transform->getLocalRotation();
        scene->createNode()->addComponent<SpawnedObject>(effect, mesh, initialPos, initialRotation);
    }

    Device *device = nullptr;
    Scene *scene = nullptr;
    Transform *transform = nullptr;
    sptr<Material> material;
    sptr<Mesh> mesh;
    sptr<Effect> effect;
};


class Demo final
{
public:
    explicit Demo(Device* device):
        device(device),
        scene(device->getScene()),
        loader(device->getAssetLoader())
    {
        initObjects();
        initCamera();
        initSkybox();
    }

    void update()
    {
        scene->visit([](Component *cmp) { cmp->update(); });
    }

    void render()
    {
        camera->apply([&](const RenderContext& ctx)
        {
            renderByTags(skyboxTag, ctx);
            renderByTags(~skyboxTag, ctx);
        });
    }

private:
    void renderByTags(uint32_t tags, const RenderContext &ctx)
    {
        scene->visit(tags, [=](Component *cmp) { cmp->render(ctx); });
    }

    void initCamera()
    {
        auto node = scene->createNode();
        auto t = node->findComponent<Transform>();
        t->setLocalPosition(Vector3(10, 10, 10));
        t->lookAt(Vector3::zero(), Vector3::unitY());

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
        node->findComponent<Transform>()->setLocalScale(Vector3(10, 0.1f, 10));
        auto renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(cubeMesh);
        renderer->setMaterial(0, mat);

        auto rigidBody = node->addComponent<RigidBody>(RigidBodyConstructionParameters().withMass(0).withFriction(0.5f));
        rigidBody->setCollider(BoxCollider::create(Vector3::unit()));
    }

    const uint32_t skyboxTag = 1 << 2;

    Device *device = nullptr;
    Scene *scene = nullptr;
    AssetLoader *loader = nullptr;
    Camera *camera = nullptr;
    sptr<Mesh> cubeMesh;
};


int main()
{
    auto device = Device::create(DeviceSetup().withMode(DeviceMode::OpenGL).withDimensions(1200, 600).withLogFilePath("demo4.log").withWindowTitle("Demo 4"));
    Demo demo(device.get());
    while (!device->isQuitRequested() && !device->isWindowCloseRequested() && !device->isKeyPressed(KeyCode::Escape, true))
    {
        device->update([&]()
        {
            device->getAssetLoader()->update();
            device->getPhysics()->update();
            device->getRenderer()->renderFrame([&]()
            {
                demo.update();
                demo.render();
            });
        });
    }
    return 0;
}
