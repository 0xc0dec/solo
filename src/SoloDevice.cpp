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
#include "platform/vulkan/SoloSDLVulkanDevice.h"

using namespace solo;


// The whole system is made as singleton for the sake of coding convenience.
// For instance, to make resource creation look like Resource::create(args)
// instead of device->getResourceManager()->createResource(args)
uptr<Device> Device::instance = nullptr;


auto Device::get() -> Device*
{
    return instance.get();
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
}


uptr<Device> Device::createInstance(const DeviceCreationArgs& args)
{
    switch (args.mode)
    {
        case DeviceMode::OpenGL:
            return std::make_unique<SDLOpenGLDevice>(args);
        case DeviceMode::Vulkan:
            return std::make_unique<SDLVulkanDevice>(args);
        case DeviceMode::Stub:
            return std::make_unique<StubDevice>(args);
        default:
            SL_ASSERT(false);
            return nullptr;
    }
}


void Device::init()
{
    logger = std::make_unique<Logger>(DeviceToken());
    if (!creationArgs.logFilePath.empty())
        instance->logger->setTargetFile(creationArgs.logFilePath);

    DeviceToken token;
    renderer = Renderer::create(this, token);
    physics = Physics::create(this, token);
    fs = FileSystem::create(this, token);
    assetLoader = std::make_unique<AssetLoader>(token);
    graphics = std::make_unique<Graphics>(this, token);
    scene = std::make_unique<Scene>(token);
}


void Device::run()
{
    do
    {
        beginUpdate();
        assetLoader->update();
        physics->update();
        scene->update();
        scene->render();
        endUpdate();
    } while (running);
}


void Device::run(const DeviceCreationArgs& args, sptr<DeviceCallback> callback)
{
    SL_ASSERT(!instance);
    instance = createInstance(args);
    instance->init();

    callback->onStarted();
    instance->run();
    callback.reset();

    instance.reset();
    instance = nullptr;
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
