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

#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloAssetLoader.h"
#include "SoloScene.h"
#include "SoloLogger.h"
#include "SoloRenderer.h"
#include "SoloGraphics.h"
#include "SoloPhysics.h"
#include "platform/null/SoloNullDevice.h"
#include "platform/opengl/SoloSDLOpenGLDevice.h"

using namespace solo;


// Made as a singleton for the sake of convenience.
// Instantiated when run() is called and resets when it exists
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


Device::Device(const DeviceSetup& setup):
    setup(setup)
{
}


uptr<Device> Device::createInstance(const DeviceSetup& setup)
{
    switch (setup.mode)
    {
        case DeviceMode::OpenGL:
            return std::make_unique<SDLOpenGLDevice>(setup);
        case DeviceMode::Null:
            return std::make_unique<NullDevice>(setup);
        default:
            SL_ERR("Unknown device mode");
            return nullptr;
    }
}


void Device::init()
{
    DeviceToken token;

    logger = std::make_unique<Logger>(token);
    if (!setup.logFilePath.empty())
        instance->logger->setTargetFile(setup.logFilePath);
    
    renderer = Renderer::create(this, token);
    physics = Physics::create(this, token);
    fs = FileSystem::create(this, token);
    assetLoader = std::make_unique<AssetLoader>(token);
    graphics = Graphics::create(this, token);
    scene = std::make_unique<Scene>(token);
}


void Device::run()
{
    do
    {
        beginUpdate();
        assetLoader->update();
        physics->update();
        renderer->beginFrame();
        scene->update();
        scene->render();
        renderer->endFrame();
        endUpdate();
    } while (running);
}


void Device::run(const DeviceSetup& setup, sptr<DeviceCallback> callback)
{
    SL_ERR_IF(instance, "Device instance is already running");
    instance = createInstance(setup);
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
