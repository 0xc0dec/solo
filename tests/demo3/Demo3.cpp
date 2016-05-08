#include "../../include/Solo.h"
#include "../common/EscapeWatcher.h"
#include "../common/Screenshoter.h"
#include "../common/Shaders.h"
#include <chrono>
#include <iomanip>

using namespace solo;


class Text final: public ComponentBase<Text>
{
public:
    explicit Text(const Node& node): ComponentBase<Text>(node)
    {
    }

    virtual void init() override final
    {
        const int textureWidth = 1024;
        const int textureHeight = 1024;
        const int lineHeight = 60;

        auto fontData = Device::get()->getFileSystem()->readBytes("c:/windows/fonts/calibri.ttf");
        auto font = Font::create(fontData.data(), lineHeight, textureWidth, textureHeight, ' ', '~' - ' ', 2, 2);

        renderer = node.addComponent<FontRenderer>();
        renderer->setFont(font);
    }

    virtual void update() override final
    {
        auto nowTimePoint = std::chrono::system_clock::now();
        auto now = std::chrono::system_clock::to_time_t(nowTimePoint);
        std::stringstream ss;
        ss << "Now: " << std::put_time(std::localtime(&now), "%X");
        renderer->setText(ss.str());
    }

private:
    FontRenderer* renderer = nullptr;
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
        initText();
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

    void initText()
    {
        auto textNode = scene->createNode();
        textNode->addComponent<Text>();
        auto transform = textNode->getComponent<Transform>();
        transform->setLocalPosition(Vector3(-3.5f, 2, 0));
        transform->setLocalScale(Vector3(0.02f, 0.02f, 1));
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
