#include "../../include/Solo.h"
#include "../common/EscapeWatcher.h"
#include "../common/Screenshoter.h"
#include "../common/Shaders.h"

using namespace solo;


class SpawnedObject: public ComponentBase<SpawnedObject>
{
public:
    explicit SpawnedObject(const Node& node, sptr<Material> material, sptr<Mesh> mesh,
        const Vector3& initialPos, const Quaternion& initialRotation):
        ComponentBase<SpawnedObject>(node),
        scene(node.getScene()),
        initialPos(initialPos),
        initialRotation(initialRotation),
        material(material),
        mesh(mesh)
    {
    }

    void setColor(const Vector4& color)
    {
        material->setVector4Parameter("color", color);
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

        auto rigidBodyParams = RigidBodyConstructionParameters();
        rigidBodyParams.mass = 10;
        rigidBodyParams.restitution = 0.01f;
        rigidBodyParams.friction = 0.01f;
        auto rigidBody = node.addComponent<RigidBody>(rigidBodyParams);
        rigidBody->setCollider(BoxCollider::create(Vector3::unit()));
    }

private:
    Scene* scene;
    Vector3 initialPos;
    Quaternion initialRotation;
    sptr<Material> material;
    sptr<Mesh> mesh;
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
        auto result = physics->castRay(
            transform->getWorldPosition(),
            transform->getWorldPosition() + transform->getLocalForward() * 100);

        auto obj = result.rigidBody ? result.rigidBody->getNode().findComponent<SpawnedObject>() : nullptr;
        if (obj == prevObj)
            return;

        if (prevObj)
            prevObj->setColor(normalColor);
        if (obj)
            obj->setColor(activeColor);
        prevObj = obj;
    }

private:
    Vector4 normalColor = Vector4(1, 1, 0, 1);
    Vector4 activeColor = Vector4(1, 0, 0, 1);
    Physics* physics = nullptr;
    Transform* transform = nullptr;
    SpawnedObject* prevObj = nullptr;
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
        auto effect = Effect::create(commonShaders.vertex.basic, commonShaders.fragment.color);
        material = Material::create(effect);
        material->setFaceCull(FaceCull::All);
        material->setPolygonMode(PolygonMode::Wireframe);
        material->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        material->setVector4Parameter("color", Vector4(1, 1, 0, 1));
    }

    void update() override final
    {
        if (Device::get()->isMouseButtonDown(MouseButton::Right, true))
            spawn();
    }

private:
    void spawn()
    {
        auto initialPos = transform->getLocalPosition() + transform->getLocalForward() * 2;
        auto initialRotation = transform->getLocalRotation();
        scene->createNode()->addComponent<SpawnedObject>(material, mesh, initialPos, initialRotation);
    }

    Device* device;
    Scene* scene;
    Transform* transform;
    sptr<Material> material;
    sptr<Mesh> mesh;
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
        device = Device::init(DeviceCreationArgs().withMode(DeviceMode::OpenGL).withDimensions(1200, 600).withLogPath("demo4.log"));
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

        auto rigidBodyParams = RigidBodyConstructionParameters();
        rigidBodyParams.mass = 0;
        auto rigidBody = node->addComponent<RigidBody>(rigidBodyParams);
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
