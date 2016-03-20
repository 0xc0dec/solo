#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloResourceManager.h"
#include "SoloScene.h"
#include "SoloLogger.h"
#include "SoloRenderer.h"
#include "SoloGraphics.h"
#include "platform/stub/SoloStubDevice.h"
#include "platform/opengl/SoloSDLOpenGLDevice.h"

using namespace solo;


shared<Device> Device::create(const DeviceCreationArgs& args)
{
    if (args.mode == DeviceMode::OpenGL)
        return SL_NEW_SHARED(SDLOpenGLDevice, args);
    return SL_NEW_SHARED(StubDevice, args);
}


Device::~Device()
{
    cleanup();
}


Device::Device(const DeviceCreationArgs& args):
    creationArgs(args)
{
    logger = SL_NEW_SHARED(Logger);
    if (!creationArgs.logFilePath.empty())
        logger->setTargetFile(creationArgs.logFilePath);

    renderer = Renderer::create(this);
    fs = FileSystem::create(this);
    resourceManager = ResourceManager::create(this);
    graphics = SL_NEW_SHARED(Graphics, this);
    scene = SL_NEW_SHARED(Scene, this);
}


void Device::run()
{
    // By design you are allowed to call this method again and again
    // as long as the engine object is alive. Once the engine object has been destroyed (or had it's shutdown()
    // method called), this method will no longer work, because the object is considered disposed.
    running = true;
    while (true)
    {
        beginUpdate();
        scene->update();
        scene->render();
        endUpdate();
        if (!running)
            break;
    }
}


void Device::shutdown()
{
    if (running)
        SL_THROW(InvalidOperationException, "Cannot shut down engine while it is running");
    cleanup();
}


bool Device::isKeyPressed(KeyCode code, bool firstTimeOnly) const
{
    auto where = pressedKeys.find(code);
    return where != pressedKeys.end() && (!firstTimeOnly || where->second);
}


bool Device::isKeyReleased(KeyCode code) const
{
    return releasedKeys.find(code) != releasedKeys.end();
}


Vector2 Device::getMouseMotion() const
{
    return Vector2(static_cast<float>(mouseDeltaX), static_cast<float>(mouseDeltaY));
}


bool Device::isMouseButtonDown(MouseButton button, bool firstTimeOnly) const
{
    auto where = pressedMouseButtons.find(button);
    return where != pressedMouseButtons.end() && (!firstTimeOnly || where->second);
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


void Device::cleanup()
{
    if (scene)
    {
        scene.reset();
        scene = nullptr;
    }

    if (graphics)
    {
        graphics.reset();
        graphics = nullptr;
    }

    if (resourceManager)
    {
        resourceManager.reset();
        resourceManager = nullptr;
    }

    if (fs)
    {
        fs.reset();
        fs = nullptr;
    }

    if (renderer)
    {
        renderer.reset();
        renderer = nullptr;
    }

    if (logger)
    {
        logger.reset();
        logger = nullptr;
    }
}