#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloAssetLoader.h"
#include "SoloScene.h"
#include "SoloLogger.h"
#include "SoloRenderer.h"
#include "SoloGraphics.h"
#include "SoloPhysics.h"
#include "platform/stub/SoloStubDevice.h"
#include "platform/opengl/SoloSDLOpenGLDevice.h"

using namespace solo;


uptr<Device> Device::instance = nullptr;


auto Device::init(const DeviceCreationArgs& args) -> Device*
{
    if (!instance)
    {
        if (args.mode == DeviceMode::OpenGL)
            instance = std::unique_ptr<Device>(new SDLOpenGLDevice(args));
        else
            instance = std::unique_ptr<Device>(new StubDevice(args));
    }

    return instance.get();
}


auto Device::get() -> Device*
{
    return instance.get();
}


void Device::shutdown()
{
    if (instance)
        instance = nullptr;
}


Device::~Device()
{
    // Keep order!

    if (scene)
        scene.reset();

    if (graphics)
        graphics.reset();

    if (assetLoader)
        assetLoader.reset();

    if (fs)
        fs.reset();

    if (physics)
        physics.reset();

    if (renderer)
        renderer.reset();

    if (logger)
        logger.reset();
}


Device::Device(const DeviceCreationArgs& args):
    creationArgs(args)
{
    logger = std::make_unique<Logger>(DeviceToken());
    if (!args.logFilePath.empty())
        logger->setTargetFile(args.logFilePath);

    auto token = DeviceToken();
    renderer = Renderer::create(this, token);
    physics = Physics::create(this, token);
    fs = FileSystem::create(this, token);
    assetLoader = std::make_unique<AssetLoader>(token);
    graphics = std::make_unique<Graphics>(this, token);
    scene = std::make_unique<Scene>(token);
}


void Device::run()
{
    // By design you are allowed to call this method again and again
    // as long as the engine object is alive. Once the engine object has been destroyed (or had it's shutdown()
    // method called), this method will no longer work, because the object is considered disposed.
    running = true;
    while (true)
    {
        assetLoader->update();
        beginUpdate();
        physics->update();
        scene->update();
        scene->render();
        endUpdate();
        if (!running)
            break;
    }
}


bool Device::isKeyPressed(KeyCode code, bool firstTime) const
{
    auto where = pressedKeys.find(code);
    return where != pressedKeys.end() && (!firstTime || where->second);
}


bool Device::isKeyReleased(KeyCode code) const
{
    return releasedKeys.find(code) != releasedKeys.end();
}


auto Device::getMouseMotion() const -> Vector2
{
    return Vector2(static_cast<float>(mouseDeltaX), static_cast<float>(mouseDeltaY));
}


bool Device::isMouseButtonDown(MouseButton button, bool firstTime) const
{
    auto where = pressedMouseButtons.find(button);
    return where != pressedMouseButtons.end() && (!firstTime || where->second);
}


bool Device::isMouseButtonReleased(MouseButton button) const
{
    return releasedMouseButtons.find(button) != releasedMouseButtons.end();
}


void Device::updateTime()
{
    auto time = getLifetime();
    timeDelta = time - lastUpdateTime;
    lastUpdateTime = time;
}
