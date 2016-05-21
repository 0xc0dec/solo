#include "../../include/Solo.h"
#include "../common/EscapeWatcher.h"
#include "../common/Screenshoter.h"
#include "../common/Shaders.h"

using namespace solo;


class Demo
{
public:
    void run()
    {
        initEngine();
        initCamera();
        initSkybox();
        initObjects();
        device->run();
    }

    void initEngine()
    {
        DeviceCreationArgs args;
        args.mode = DeviceMode::OpenGL;
        args.canvasWidth = 1200;
        args.canvasHeight = 600;
        args.logFilePath = "demo3.log";
        device = Device::init(args);

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

        cubeMaterial = Material::create(Effect::create(commonShaders.vertex.basic, commonShaders.fragment.texture));
        cubeMaterial->setFaceCull(FaceCull::All);
        cubeMaterial->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
        cubeMaterial->setTextureParameter("mainTex", tex);

        cubeMesh = Mesh::create(MeshPrefab::Cube);

        // Falling cube
        auto node = scene->createNode();
        auto renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(cubeMesh);
        renderer->setMaterial(0, cubeMaterial);
        node->getComponent<Transform>()->setLocalPosition(Vector3(1.5f, 10, 1));
        node->getComponent<Transform>()->setLocalRotation(Vector3::unit(), Degree(45));

        auto rigidBodyParams = RigidBodyConstructionParameters();
        rigidBodyParams.mass = 10;
        rigidBodyParams.restitution = 0.01f;
        rigidBodyParams.friction = 0.01f;
        auto rigidBody = node->addComponent<RigidBody>(rigidBodyParams);
        rigidBody->setCollider(BoxCollider::create(Vector3::unit()));

        // Floor
        node = scene->createNode();
        node->getComponent<Transform>()->setLocalScale(Vector3(10, 0.1f, 10));
        renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(cubeMesh);
        renderer->setMaterial(0, cubeMaterial);

        rigidBodyParams = RigidBodyConstructionParameters();
        rigidBodyParams.mass = 0;
        rigidBody = node->addComponent<RigidBody>(rigidBodyParams);
        rigidBody->setCollider(BoxCollider::create(Vector3::unit()));
    }

private:
    Scene* scene = nullptr;
    AssetLoader* loader = nullptr;
    Device* device = nullptr;
    sptr<Material> cubeMaterial;
    sptr<Mesh> cubeMesh;
};


int main()
{
    Demo().run();
    return 0;
}
