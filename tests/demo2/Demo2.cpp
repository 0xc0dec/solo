#include "../../include/Solo.h"
#include "../common/EscapeWatcher.h"
#include "../common/Screenshoter.h"
#include "../common/Rotator.h"
#include "../common/Shaders.h"
#include "Shaders.h"

using namespace solo;


class PostProcessor1 final: public ComponentBase<PostProcessor1>
{
public:
    explicit PostProcessor1(const Node& node):
        ComponentBase<PostProcessor1>(node),
        device(Device::get()),
        loader(Device::get()->getAssetLoader()),
        graphics(Device::get()->getGraphics())
    {
    }

    virtual void init() override final
    {
        auto canvasSize = device->getCanvasSize();

        auto camera = node.getComponent<Camera>();
        fbTex = Texture2D::create();
        fbTex->setData(TextureFormat::RGB, {}, static_cast<uint32_t>(canvasSize.x), static_cast<uint32_t>(canvasSize.y));
        fbTex->setFiltering(TextureFiltering::Nearest);
        fbTex->setWrapping(TextureWrapping::Clamp);
        fb1 = FrameBuffer::create();
        fb1->setAttachments({ fbTex });
        camera->setRenderTarget(fb1);

        fbTex2 = Texture2D::create();
        fbTex2->setData(TextureFormat::RGB, {}, static_cast<uint32_t>(canvasSize.x), static_cast<uint32_t>(canvasSize.y));
        fbTex2->setFiltering(TextureFiltering::Nearest);
        fbTex2->setWrapping(TextureWrapping::Clamp);
        fb2 = FrameBuffer::create();
        fb2->setAttachments({ fbTex2 });

        auto grayscaleEffect = Effect::create(commonShaders.vertex.passThrough, fsGrayscale);
        grayscaleMat = Material::create(grayscaleEffect);
        grayscaleMat->setFloatParameter("rightSeparator", 0.25f);

        auto saturateEffect = Effect::create(commonShaders.vertex.passThrough, fsSaturate);
        saturateMat = Material::create(saturateEffect);
        saturateMat->setFloatParameter("leftSeparator", 0.75f);
        saturateMat->setFloatParameter("rightSeparator", 1.0f);

        auto verticalBlurEffect = Effect::create(commonShaders.vertex.passThrough, fsVerticalBlur);
        verticalBlurMat = Material::create(verticalBlurEffect);
        verticalBlurMat->setFloatParameter("leftSeparator", 0.25f);
        verticalBlurMat->setFloatParameter("rightSeparator", 0.75f);

        auto horizontalBlurEffect = Effect::create(commonShaders.vertex.passThrough, fsHorizontalBlur);
        horizontalBlurMat = Material::create(horizontalBlurEffect);
        horizontalBlurMat->setFloatParameter("leftSeparator", 0.25f);
        horizontalBlurMat->setFloatParameter("rightSeparator", 0.75f);
    }

    virtual void terminate() override final
    {
        node.getComponent<Camera>()->setRenderTarget(nullptr);
    }

    virtual void onAfterCameraRender() override final
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
    Device* device;
    AssetLoader* loader;
    Graphics* graphics;
    sptr<FrameBuffer> fb1 = nullptr;
    sptr<FrameBuffer> fb2 = nullptr;
    sptr<Texture2D> fbTex = nullptr;
    sptr<Texture2D> fbTex2 = nullptr;
    sptr<Material> grayscaleMat = nullptr;
    sptr<Material> saturateMat = nullptr;
    sptr<Material> verticalBlurMat = nullptr;
    sptr<Material> horizontalBlurMat = nullptr;
};


class PostProcessor2 final: public ComponentBase<PostProcessor2>
{
public:
    explicit PostProcessor2(const Node& node) :
        ComponentBase<PostProcessor2>(node),
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
            Math::clamp(static_cast<int>(canvasSize.y / stitchTexSize.y) * 2, 1, 2048));

        auto stitchCount = Vector2(
            resolution.x * stitchWidth / (2 * stitchTexSize.x),
            resolution.y / 2);

        auto camera = node.getComponent<Camera>();
        fbTex = Texture2D::create();
        fbTex->setData(TextureFormat::RGB, {}, static_cast<uint32_t>(resolution.x), static_cast<uint32_t>(resolution.y));
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

    virtual void terminate() override final
    {
        auto canvasSize = device->getCanvasSize();
        auto camera = node.getComponent<Camera>();
        camera->setRenderTarget(nullptr);
        camera->setViewport(0, 0, canvasSize.x, canvasSize.y);
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


class ModeSwitcher final: public ComponentBase<ModeSwitcher>
{
public:
    explicit ModeSwitcher(const Node& node, Device* device):
        ComponentBase<ModeSwitcher>(node),
        device(device)
    {
    }

    virtual void update() override final
    {
        if (device->isKeyPressed(KeyCode::Digit1, true))
        {
            if (node.findComponent<PostProcessor1>())
                return;
            node.removeComponent<PostProcessor2>();
            node.addComponent<PostProcessor1>();
        }
        if (device->isKeyPressed(KeyCode::Digit2, true))
        {
            if (node.findComponent<PostProcessor2>())
                return;
            node.removeComponent<PostProcessor1>();
            node.addComponent<PostProcessor2>();
        }
        if (device->isKeyPressed(KeyCode::Digit3, true))
        {
            node.removeComponent<PostProcessor1>();
            node.removeComponent<PostProcessor2>();
        }
    }

private:
    Device* device;
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
        device->getLogger()->logInfo("Press keys 1..3 to switch between modes");
        device->run();
    }

    void initEngine()
    {
        DeviceCreationArgs args;
        args.mode = DeviceMode::OpenGL;
        args.canvasWidth = 1200;
        args.canvasHeight = 600;
        args.logFilePath = "demo2.log";
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
        node->addComponent<PostProcessor2>();
        node->addComponent<ModeSwitcher>(device);
        node->addComponent<Screenshoter>("demo2-screenshot.bmp");
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
            mat->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
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
