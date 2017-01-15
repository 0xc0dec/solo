/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "../common/DemoBase.h"
#include "../common/Screenshoter.h"
#include <chrono>
#include <iomanip>

using namespace solo;


class Text final: public ComponentBase<Text>
{
public:
    explicit Text(const Node &node):
        ComponentBase<Text>(node),
        device(node.getScene()->getDevice())
    {
    }

    void init() override final
    {
        const int textureWidth = 1024;
        const int textureHeight = 1024;
        const int lineHeight = 60;

        auto fontData = device->getFileSystem()->readBytes("../assets/aller.ttf");
        auto font = Font::create(device, fontData.data(), lineHeight, textureWidth, textureHeight, ' ', '~' - ' ', 2, 2);

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
    Device *device = nullptr;
    FontRenderer *renderer = nullptr;
};


class Demo final: public DemoBase
{
public:
    explicit Demo(Device *device): DemoBase(device)
    {
        initCamera();
        initSkybox();
        initText();
    }

private:
    void render() override final
    {
        camera->render([&](const RenderContext& ctx)
        {
            renderByTags(skyboxTag, ctx);
            renderByTags(~skyboxTag, ctx);
        });
    }

    void initCamera()
    {
        auto node = scene->createNode();
        auto t = node->findComponent<Transform>();
        t->setLocalPosition(Vector3(0, 0, 5));
        camera = node->addComponent<Camera>();
        camera->setClearColor(0.0f, 0.6f, 0.6f, 1.0f);
        camera->setNear(0.05f);
        node->addComponent<Screenshoter>("demo3-screenshot.bmp");

        node->addComponent<Spectator>();
    }

    void initSkybox()
    {
        loader->loadCubeTextureAsync(
        {
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
            renderer->setTags(skyboxTag);
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

    const uint32_t skyboxTag = 1 << 2;

    Camera *camera = nullptr;
};


int main()
{
    auto device = Device::create(DeviceSetup()
        .withMode(DeviceMode::OpenGL)
        .withDimensions(1200, 600)
        .withLogFilePath("demo3.log")
        .withWindowTitle("Demo 3"));
    Demo(device.get()).run();
    return 0;
}
