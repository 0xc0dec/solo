#include "../../include/Solo.h"
#include "../common/EscapeWatcher.h"
#include "../common/Screenshoter.h"
#include "../common/Shaders.h"

using namespace solo;


class SpawnedObject: public ComponentBase<SpawnedObject>
{
public:
    explicit SpawnedObject(const Node& node, sptr<Effect> effect, sptr<Mesh> mesh,
        const Vector3& initialPos, const Quaternion& initialRotation):
        ComponentBase<SpawnedObject>(node),
        device(Device::get()),
        scene(node.getScene()),
        initialPos(initialPos),
        initialRotation(initialRotation),
        mesh(mesh)
    {
        material = Material::create(effect);
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

    void init() override
    {
        auto renderer = node.addComponent<MeshRenderer>();
        renderer->setMesh(mesh);
        renderer->setMaterial(0, material);
        auto t = node.getComponent<Transform>();
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

    void update() override
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
    void setColor(const Vector4& color)
    {
        material->setVector4Parameter("color", color);
    }

    Device* device;
    Scene* scene;
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
    explicit Targeter(const Node& node):
        ComponentBase<Targeter>(node),
        physics(Device::get()->getPhysics())
    {
    }

    void init() override
    {
        transform = node.getComponent<Transform>();
    }

    void update() override
    {
        auto hitResults = physics->castRayAll(
            transform->getWorldPosition(),
            transform->getWorldPosition() + transform->getLocalForward() * 100);

        for (const auto& result : hitResults)
        {
            auto obj = result.body->getNode().findComponent<SpawnedObject>();
            if (obj)
                obj->setActive();
        }
    }

private:
    Vector4 normalColor = Vector4(1, 1, 0, 1);
    Vector4 activeColor = Vector4(1, 0, 0, 1);
    Physics* physics = nullptr;
    Transform* transform = nullptr;
};


class Spawner final: public ComponentBase<Spawner>
{
public:
    explicit Spawner(const Node& node, sptr<Mesh> cubeMesh):
        ComponentBase<Spawner>(node),
        device(Device::get()),
        scene(Device::get()->getScene()),
        transform(node.getComponent<Transform>()),
        mesh(cubeMesh)
    {
    }

    void init() override final
    {
        effect = Effect::create(commonShaders.vertex.basic, commonShaders.fragment.color);
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

    Device* device;
    Scene* scene;
    Transform* transform;
    sptr<Material> material;
    sptr<Mesh> mesh;
    sptr<Effect> effect;
};


class Demo
{
public:
    void run()
    {
        initEngine();
        initObjects();
        initCamera();
        initSkybox();
        device->run();
    }

    void initEngine()
    {
        device = Device::init(DeviceCreationArgs().withMode(DeviceMode::OpenGL).withDimensions(1200, 600).withLogFilePath("demo4.log"));
        scene = device->getScene();
        loader = device->getAssetLoader();
    }

    void initCamera()
    {
        auto node = scene->createNode();
        auto t = node->getComponent<Transform>();
        t->setLocalPosition(Vector3(10, 10, 10));
        t->lookAt(Vector3::zero(), Vector3::unitY());
        auto cam = node->addComponent<Camera>();
        cam->setClearColor(0.0f, 0.6f, 0.6f, 1.0f);
        cam->setNear(0.05f);
        node->addComponent<Spectator>();
        node->addComponent<EscapeWatcher>();
        node->addComponent<Screenshoter>("demo3-screenshot.bmp");
        node->addComponent<Spawner>(cubeMesh);
        node->addComponent<Targeter>();
    }

    void initSkybox()
    {
        loader->loadCubeTextureAsync({
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
        });
    }

    void initObjects()
    {
        auto tex = loader->loadRectTexture("../assets/cobblestone.png");
        tex->setWrapping(TextureWrapping::Clamp);
        tex->generateMipmaps();

        auto mat = Material::create(Effect::create(commonShaders.vertex.basic, commonShaders.fragment.texture));
        mat->setFaceCull(FaceCull::All);
        mat->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        mat->setTextureParameter("mainTex", tex);

        cubeMesh = Mesh::create(MeshPrefab::Cube);

        // Floor
        auto node = scene->createNode();
        node->getComponent<Transform>()->setLocalScale(Vector3(10, 0.1f, 10));
        auto renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(cubeMesh);
        renderer->setMaterial(0, mat);

        auto rigidBody = node->addComponent<RigidBody>(RigidBodyConstructionParameters().withMass(0).withFriction(0.5f));
        rigidBody->setCollider(BoxCollider::create(Vector3::unit()));
    }

private:
    Scene* scene = nullptr;
    AssetLoader* loader = nullptr;
    Device* device = nullptr;
    sptr<Mesh> cubeMesh;
};


int main()
{
    Demo().run();
    return 0;
}
