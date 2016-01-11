#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloResourceManager.h"
#include "SoloScene.h"
#include "SoloLogger.h"
#include "platform/stub/SoloStubDevice.h"
#include "platform/opengl/SoloSDLOpenGLDevice.h"

using namespace solo;


shared<Device> Device::create(const DeviceCreationArgs &args)
{
    if (args.mode == DeviceMode::OpenGL)
        return SL_NEW_SHARED(SDLOpenGLDevice, args);
    return SL_NEW_SHARED(StubDevice, args);
}


Device::Device(const DeviceCreationArgs &args):
    creationArgs(args)
{
    logger = Logger::create();
    if (!args.logFilePath.empty())
        logger->setTargetFile(args.logFilePath);

    fs = FileSystem::create(this);
    resourceManager = ResourceManager::create(this);
    scene = Scene::create(this);
}


void Device::run()
{
    startCallback();

    while (true)
    {
        beginUpdate();
        scene->update();
        scene->render();
        endUpdate();
        if (shutdown && shutdownRequestedCallback())
            break;
    }

    shutdownCallback();
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
