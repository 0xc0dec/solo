#include "../../include/Solo.h"
#include "../common/EscapeWatcher.h"
#include "../common/Screenshoter.h"
#include "../common/Rotator.h"
#include "../common/Shaders.h"
#include "Shaders.h"

using namespace solo;


class Targeter: public ComponentBase<Targeter>
{
public:
    explicit Targeter(const Node& node, Vector3 targetPos):
        ComponentBase<Targeter>(node),
        targetPos(targetPos)
    {
    }

    virtual void init() override final
    {
        transform = node.getComponent<Transform>();
    }

    virtual void update() override final
    {
        transform->lookAt(targetPos, Vector3::unitY());
    }

private:
    Vector3 targetPos;
    Transform* transform = nullptr;
};


class DynamicQuadUpdater: public ComponentBase<DynamicQuadUpdater>
{
public:
    explicit DynamicQuadUpdater(const Node& node, std::vector<float> data, sptr<Mesh> mesh):
        ComponentBase<DynamicQuadUpdater>(node),
        data(data),
        device(solo::Device::get()),
        mesh(mesh)
    {
    }

    virtual void update() override final
    {
        time += 2 * device->getTimeDelta();
        auto offset = 0.3f * sin(time);
        data[2] = offset;
        data[7] = -offset;
        data[12] = offset;
        data[17] = -offset;
        mesh->updateDynamicVertexBuffer(0, 0, data.data(), 4);
    }

private:
    float time = 0;
    std::vector<float> data;
    Device* device;
    sptr<Mesh> mesh;
};


class Demo
{
public:
    void run()
    {
        initEngine();
        initEffects();
        initCamera();
        initOffscreenCamera();
        initMaterials();
        initSkybox();
        initCheckerBox();
        initMesh();
        initDynamicQuad();
        initAxesMesh()->done([&] (sptr<Mesh> mesh)
        {
            axesMesh = mesh;
            initMonitorQuad(Vector3());
            initTransparentQuad();
        });
        device->run();
    }

    void initEngine()
    {
        DeviceCreationArgs args;
        args.mode = DeviceMode::OpenGL;
        args.canvasWidth = 1200;
        args.canvasHeight = 600;
        args.logFilePath = "demo1.log";
        device = Device::init(args);

        scene = device->getScene();
        loader = device->getAssetLoader();
        canvasSize = device->getCanvasSize();
    }

    sptr<Material> createColorMaterial(const Vector4& color)
    {
        auto mat = Material::create(colorEffect);
        mat->setPolygonFace(PolygonFace::All);
        mat->setParameterAutoBinding("worldViewProjMatrix", AutoBinding::WorldViewProjectionMatrix);
        mat->setVector4Parameter("color", color);
        return mat;
    }

    void initEffects()
    {
        simpleTextureEffect = Effect::create(commonShaders.vertex.basic, commonShaders.fragment.texture);
        colorEffect = Effect::create(commonShaders.vertex.basic, fsColor);
        checkerEffect = Effect::create(commonShaders.vertex.basic, fsChecker);
        texWithLightingEffect = Effect::create(vsBasicLighting, fsTextureWithLighting);
    }

    void loadTexture(const std::string& path, std::function<void(sptr<Texture2D>)> callback)
    {
        loader->loadTexture2DAsync(path)->done([=](sptr<Texture2D> tex)
        {
            tex->generateMipmaps();
            tex->setFiltering(TextureFiltering::LinearMipmapNearest);
            tex->setAnisotropyLevel(8);
            callback(tex);
        });
    }

    sptr<AsyncHandle<Mesh>> initAxesMesh()
    {
        return loader->loadMeshAsync("../assets/axes.obj");
    }

    void initCamera()
    {
        auto node = scene->createNode();
        auto t = node->getComponent<Transform>();
        t->setLocalPosition(Vector3(0, 5, 10));
        t->lookAt(Vector3::zero(), Vector3::unitY());
        node->addComponent<Spectator>();
        node->addComponent<EscapeWatcher>();
        node->addComponent<Screenshoter>("demo1-screenshot.bmp");
        auto cam = node->addComponent<Camera>();
        cam->setClearColor(0.0f, 0.6f, 0.6f, 1.0f);
        cam->setNear(0.05f);
    }

    void initOffscreenCamera()
    {
        offscreenCameraTex = Texture2D::create();
        offscreenCameraTex->setData(TextureFormat::RGB, {}, static_cast<uint32_t>(floor(canvasSize.x / 8.0f)), static_cast<uint32_t>(floor(canvasSize.y / 8.0f)));
        offscreenCameraTex->setFiltering(TextureFiltering::Nearest);
        offscreenCameraTex->setWrapping(TextureWrapping::Clamp);

        auto node = scene->createNode();
        auto cam = node->addComponent<Camera>();
        cam->setClearColor(1, 0, 1, 1);
        cam->setNear(0.05f);
        cam->setViewport(0, 0, canvasSize.x / 8, canvasSize.y / 8);
        cam->getRenderTags() = ~renderTargetQuadTag;
        node->getComponent<Transform>()->setLocalPosition(Vector3(0, 0, 10));

        auto fb = FrameBuffer::create();
        fb->setAttachments({ offscreenCameraTex });
        cam->setRenderTarget(fb);
    }

    void initMaterials()
    {
        redMat = createColorMaterial(Vector4(1, 0, 0, 1));
        greenMat = createColorMaterial(Vector4(0, 1, 0, 1));
        blueMat = createColorMaterial(Vector4(0, 0, 1, 1));
        whiteMat = createColorMaterial(Vector4(1, 1, 1, 1));

        checkerMat = Material::create(checkerEffect);
        checkerMat->setPolygonFace(PolygonFace::All);
        checkerMat->setParameterAutoBinding("worldViewProjMatrix", AutoBinding::WorldViewProjectionMatrix);
        checkerMat->setVector4Parameter("color", Vector4(1, 1, 0, 1));

        monitorMat = Material::create(simpleTextureEffect);
        monitorMat->setPolygonFace(PolygonFace::All);
        monitorMat->setParameterAutoBinding("worldViewProjMatrix", AutoBinding::WorldViewProjectionMatrix);
        monitorMat->setTextureParameter("mainTex", offscreenCameraTex);
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

    sptr<Node> createPrefabMeshNode(const std::string& type)
    {
        sptr<Mesh> mesh = nullptr;
        if (type == "quad")
            mesh = Mesh::create(MeshPrefab::Quad);
        else if (type == "cube")
            mesh = Mesh::create(MeshPrefab::Cube);
        auto node = scene->createNode();
        node->addComponent<MeshRenderer>()->setMesh(mesh);
        return node;
    };

    void initCheckerBox()
    {
        auto node = createPrefabMeshNode("cube");
        node->getComponent<MeshRenderer>()->setMaterial(0, checkerMat);
        node->getComponent<Transform>()->setLocalPosition(Vector3(-5, 0, 0));
        node->addComponent<Rotator>("world", Vector3::unitY());
    }

    void initMesh()
    {
        loader->loadTexture2DAsync("../assets/cobblestone.png")->done([=](sptr<Texture2D> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);
            tex->generateMipmaps();

            auto mat = Material::create(texWithLightingEffect);
            mat->setPolygonFace(PolygonFace::All);
            mat->setParameterAutoBinding("worldViewProjMatrix", AutoBinding::WorldViewProjectionMatrix);
            mat->setParameterAutoBinding("invTransposedWorldMatrix", AutoBinding::InverseTransposedWorldMatrix);
            mat->setTextureParameter("mainTex", tex);

            loader->loadMeshAsync("../assets/monkey.obj")->done([=](sptr<Mesh> mesh)
            {
                auto node = scene->createNode();
                auto renderer = node->addComponent<MeshRenderer>();
                renderer->setMesh(mesh);
                renderer->setMaterial(0, mat);
                node->getComponent<Transform>()->setLocalPosition(Vector3::zero());
                node->addComponent<Rotator>("local", Vector3::unitX());
            });
        });
    }

    void initDynamicQuad()
    {
        loadTexture("../assets/freeman.png", [=](sptr<Texture2D> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);

            auto layout = VertexBufferLayout();
            layout.add(VertexBufferLayoutSemantics::Position, 3);
            layout.add(VertexBufferLayoutSemantics::TexCoord0, 2);

            std::vector<float> data =
            {
                -1, -1, 0,	0, 0,
                -1,  1, 0,	0, 1,
                1,  1, 0,	1, 1,
                1, -1, 0,	1, 0
            };

            std::vector<uint16_t> indices = 
            {
                0, 1, 2,
                0, 2, 3
            };

            auto mesh = Mesh::create();
            mesh->addDynamicVertexBuffer(layout, data.data(), 4);
            mesh->addPart(indices.data(), 6); // TODO accept uint16_t array explicitly
            mesh->setPrimitiveType(PrimitiveType::Triangles);

            auto mat = Material::create(simpleTextureEffect);
            mat->setPolygonFace(PolygonFace::All);
            mat->setParameterAutoBinding("worldViewProjMatrix", AutoBinding::WorldViewProjectionMatrix);
            mat->setTextureParameter("mainTex", tex);

            auto node = scene->createNode();
            node->getComponent<Transform>()->setLocalPosition(Vector3(0, 0, -5));
            auto renderer = node->addComponent<MeshRenderer>();
            renderer->setMesh(mesh);
            renderer->setMaterial(0, mat);

            node->addComponent<DynamicQuadUpdater>(data, mesh);
        });
    }

    void attachAxesMesh(sptr<Node> node)
    {
        auto renderer = node->addComponent<MeshRenderer>();
        renderer->setMesh(axesMesh);
        renderer->setMaterial(0, blueMat);
        renderer->setMaterial(1, greenMat);
        renderer->setMaterial(2, whiteMat);
        renderer->setMaterial(3, redMat);
    }

    void initMonitorQuad(Vector3 targetPos)
    {
        auto parent = scene->createNode();
        parent->getComponent<Transform>()->setLocalPosition(Vector3(-2, 2, -2));
        parent->addComponent<Rotator>("world", Vector3::unitY());
        attachAxesMesh(parent);

        auto quad = createPrefabMeshNode("quad");
        auto renderer = quad->getComponent<MeshRenderer>();
        renderer->setMaterial(0, monitorMat);
        renderer->getTags() = renderTargetQuadTag;
        auto transform = quad->getComponent<Transform>();
        transform->setParent(parent->getComponent<Transform>());
        transform->setLocalPosition(Vector3(5, 2, -5));
        transform->setLocalScale(Vector3(5, 5 * canvasSize.y / canvasSize.x, 1));
        quad->addComponent<Targeter>(targetPos);
    }

    void initTransparentQuad()
    {
        loadTexture("../assets/flammable.png", [=](sptr<Texture2D> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);

            auto mat = Material::create(simpleTextureEffect);
            mat->setPolygonFace(PolygonFace::All);
            mat->setParameterAutoBinding("worldViewProjMatrix", AutoBinding::WorldViewProjectionMatrix);
            mat->setTextureParameter("mainTex", tex);
            mat->setTransparent(true);

            auto parent = scene->createNode();
            parent->getComponent<Transform>()->setLocalPosition(Vector3(5, 0, 0));
            parent->addComponent<Rotator>("world", Vector3::unitY());
            attachAxesMesh(parent);

            auto quad = createPrefabMeshNode("quad");
            quad->addComponent<Rotator>("local", Vector3::unitX());
            quad->getComponent<Transform>()->setParent(parent->getComponent<Transform>());
            quad->getComponent<Transform>()->setLocalPosition(Vector3(2, 0, 0));

            auto renderer = quad->getComponent<MeshRenderer>();
            renderer->setMaterial(0, mat);
            renderer->setRenderQueue(3000);
        });
    }

private:
    const int renderTargetQuadTag = 2;
    Scene* scene = nullptr;
    AssetLoader* loader = nullptr;
    Vector2 canvasSize;
    Device* device = nullptr;
    sptr<Effect> simpleTextureEffect = nullptr;
    sptr<Effect> colorEffect = nullptr;
    sptr<Effect> checkerEffect = nullptr;
    sptr<Effect> texWithLightingEffect = nullptr;
    sptr<Mesh> axesMesh = nullptr;
    sptr<Material> redMat = nullptr;
    sptr<Material> greenMat = nullptr;
    sptr<Material> blueMat = nullptr;
    sptr<Material> whiteMat = nullptr;
    sptr<Material> checkerMat = nullptr;
    sptr<Material> monitorMat = nullptr;
    sptr<Texture2D> offscreenCameraTex = nullptr;
};


int main()
{
    Demo().run();
    return 0;
}
