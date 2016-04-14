#include "../../include/Solo.h"
#include "../common/EscapeWatcher.h"
#include "../common/Screenshoter.h"
#include "../common/Rotator.h"
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
        auto canvasSize = device->getCanvasSize();
        auto camera = node.getComponent<Camera>();
        fbTex = Texture2D::create();
        fbTex->setData(ColorFormat::RGB, {}, static_cast<uint32_t>(canvasSize.x), static_cast<uint32_t>(canvasSize.y));
        fbTex->setFiltering(TextureFiltering::Nearest);
        fbTex->setWrapping(TextureWrapping::Clamp);
        fb1 = FrameBuffer::create();
        fb1->setAttachments({ fbTex });
        camera->setRenderTarget(fb1);

        fbTex2 = Texture2D::create();
        fbTex2->setData(ColorFormat::RGB, {}, static_cast<uint32_t>(canvasSize.x), static_cast<uint32_t>(canvasSize.y));
        fbTex2->setFiltering(TextureFiltering::Nearest);
        fbTex2->setWrapping(TextureWrapping::Clamp);
        fb2 = FrameBuffer::create();
        fb2->setAttachments({ fbTex2 });

        auto grayscaleEffect = Effect::create(commonShaders.vertex.passThrough, fsGrayscale);
        material = Material::create(grayscaleEffect);
        material->setFloatParameter("rightSeparator", 0.25f);

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

    virtual void onAfterCameraRender() override final
    {
        // bounce between the two frame buffers
        material->setTextureParameter("mainTex", fbTex);
        graphics->blit(material.get(), fb2.get());

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
    sptr<Material> material = nullptr;
    sptr<Material> saturateMat = nullptr;
    sptr<Material> verticalBlurMat = nullptr;
    sptr<Material> horizontalBlurMat = nullptr;
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
        node->addComponent<PostProcessor>();
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
