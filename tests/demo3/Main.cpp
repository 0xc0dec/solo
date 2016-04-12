#include "../../include/Solo.h"
#include "../common/EscapeWatcher.h"
#include "../common/Screenshoter.h"
#include "../common/Rotator.h"
#include "../common/Shaders.h"

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
        auto imageSize = device->getCanvasSize() / 8;

        auto camera = node.getComponent<Camera>();
        fbTex1 = Texture2D::create();
        fbTex1->setData(ColorFormat::RGB, {}, static_cast<uint32_t>(imageSize.x), static_cast<uint32_t>(imageSize.y));
        fbTex1->setFiltering(TextureFiltering::Nearest);
        fbTex1->setWrapping(TextureWrapping::Clamp);
        fb1 = FrameBuffer::create();
        fb1->setAttachments({ fbTex1 });
        camera->setViewport(0, 0, imageSize.x, imageSize.y);
        camera->setRenderTarget(fb1);

        auto grayscaleEffect = Effect::create(shaders.vertex.passThrough, shaders.fragment.postProcess.grayscale);
        grayscaleMat = Material::create(grayscaleEffect);
        grayscaleMat->setFloatParameter("rightSeparator", 0.25f);
    }

    virtual void onAfterCameraRender() override final
    {
        grayscaleMat->setTextureParameter("mainTex", fbTex1);
        graphics->blit(grayscaleMat.get(), nullptr);
    }

private:
    Device* device;
    AssetLoader* loader;
    Graphics* graphics;
    sptr<FrameBuffer> fb1 = nullptr;
    sptr<Texture2D> fbTex1 = nullptr;
    sptr<Material> grayscaleMat = nullptr;
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
        node->addComponent<PostProcessor>();
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
            auto effect = Effect::create(shaders.vertex.wavy, shaders.fragment.texture);
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
                node->addComponent<Rotator>("local", Vector3::unitX());
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
