#include "../../include/Solo.h"
#include "../common/EscapeWatcher.h"
#include "../common/Screenshoter.h"
#include <chrono>
#include <iomanip>

using namespace solo;


class Text final: public ComponentBase<Text>
{
public:
    explicit Text(const Node& node): ComponentBase<Text>(node)
    {
    }

    void init() override final
    {
        const int textureWidth = 1024;
        const int textureHeight = 1024;
        const int lineHeight = 60;

        auto fontData = Device::get()->getFileSystem()->readBytes("c:/windows/fonts/calibri.ttf");
        auto font = Font::create(fontData.data(), lineHeight, textureWidth, textureHeight, ' ', '~' - ' ', 2, 2);

        renderer = node.addComponent<FontRenderer>();
        renderer->setFont(font);
    }

    void update() override final
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
        initText();
        device->run();
    }

private:
    void initEngine()
    {
        device = Device::init(DeviceCreationArgs().withMode(DeviceMode::OpenGL).withDimensions(1200, 600).withLogFilePath("demo3.log"));
        scene = device->getScene();
        loader = device->getAssetLoader();
    }

    void initCamera()
    {
        auto node = scene->createNode();
        auto t = node->findComponent<Transform>();
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
        auto transform = textNode->findComponent<Transform>();
        transform->setLocalPosition(Vector3(-3.5f, 0, 0));
        transform->setLocalScale(Vector3(0.02f, 0.02f, 1));
    }

    Scene* scene = nullptr;
    AssetLoader* loader = nullptr;
    Device* device = nullptr;
};


int main()
{
    Demo().run();
    return 0;
}
