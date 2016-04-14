#include "../../include/Solo.h"
#include "../common/EscapeWatcher.h"
#include "../common/Screenshoter.h"
#include "../common/Shaders.h"
#include "Shaders.h"

using namespace solo;


class PostProcessor: public ComponentBase<PostProcessor>
{
public:
    explicit PostProcessor(const Node& node):
        ComponentBase<PostProcessor>(node),
        device(Device::get()),
        loader(Device::get()->getAssetLoader()),
        graphics(Device::get()->getGraphics())
    {
    }

    virtual void init() override final
    {
        const float stitchWidth = 30;

        stitchTex = loader->loadTexture2D("../assets/stitches.png");
        stitchTex->setFiltering(TextureFiltering::Nearest);

        auto canvasSize = device->getCanvasSize();
        auto stitchTexSize = stitchTex->getSize();

        auto resolution = Vector2(
            Math::clamp(static_cast<int>(canvasSize.x / stitchWidth) * 2, 1, 2048),
            Math::clamp(static_cast<int>(canvasSize.y / stitchTexSize.y) * 2, 1, 2048)
        );

        auto stitchCount = Vector2(
            resolution.x * stitchWidth / (2 * stitchTexSize.x),
            resolution.y / 2
        );

        auto camera = node.getComponent<Camera>();
        fbTex = Texture2D::create();
        fbTex->setData(ColorFormat::RGB, {}, static_cast<uint32_t>(resolution.x), static_cast<uint32_t>(resolution.y));
        fbTex->setFiltering(TextureFiltering::Nearest);
        fbTex->setWrapping(TextureWrapping::Clamp);
        fb1 = FrameBuffer::create();
        fb1->setAttachments({ fbTex });
        camera->setViewport(0, 0, resolution.x, resolution.y);
        camera->setRenderTarget(fb1);

        auto effect = Effect::create(commonShaders.vertex.passThrough, fsStitches);
        material = Material::create(effect);
        material->setTextureParameter("mainTex", fbTex);
        material->setTextureParameter("stitchTex", stitchTex);
        material->setVector2Parameter("stitchCount", stitchCount);
        material->setVector2Parameter("resolution", resolution);
    }

    virtual void onAfterCameraRender() override final
    {
        graphics->blit(material.get(), nullptr);
    }

private:
    Device* device;
    AssetLoader* loader;
    Graphics* graphics;
    sptr<Texture2D> stitchTex;
    sptr<FrameBuffer> fb1;
    sptr<Texture2D> fbTex;
    sptr<Material> material;
};


class Demo
{
public:
    void run()
    {
        initEngine();
        initCamera();
        initSkybox();
        initMesh();
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
        t->setLocalPosition(Vector3(0, 0, 5));
        auto cam = node->addComponent<Camera>();
        cam->setClearColor(0.0f, 0.6f, 0.6f, 1.0f);
        cam->setNear(0.05f);
        node->addComponent<Spectator>();
        node->addComponent<EscapeWatcher>();
//        node->addComponent<PostProcessor>();
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

    void initMesh()
    {
        loader->loadTexture2DAsync("../assets/cobblestone.png")->done([=](sptr<Texture2D> tex)
        {
            tex->setWrapping(TextureWrapping::Clamp);
            tex->generateMipmaps();
            auto effect = Effect::create(commonShaders.vertex.basic, commonShaders.fragment.texture);
            auto mat = Material::create(effect);
            mat->setPolygonFace(PolygonFace::All);
            mat->setParameterAutoBinding("worldViewProjMatrix", AutoBinding::WorldViewProjectionMatrix);
            mat->setTextureParameter("mainTex", tex);

            loader->loadMeshAsync("../assets/monkey.obj")->done([=](sptr<Mesh> mesh)
            {
                auto node = scene->createNode();
                auto renderer = node->addComponent<MeshRenderer>();
                renderer->setMesh(mesh);
                renderer->setMaterial(0, mat);
                node->getComponent<Transform>()->setLocalPosition(Vector3::zero());
            });
        });
    }

private:
    Scene* scene = nullptr;
    AssetLoader* loader = nullptr;
    Device* device = nullptr;
};


int main()
{
    Demo().run();
    return 0;
}
