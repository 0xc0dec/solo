/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloScene.h"
#include "SoloLogger.h"
#include "SoloRenderer.h"
#include "SoloPhysics.h"
#include "SoloScriptRuntime.h"
#include "SoloJobPool.h"
#include "null/SoloNullDevice.h"
#include "gl/SoloSDLOpenGLDevice.h"
#include "vk/SoloSDLVulkanDevice.h"

using namespace solo;

auto Device::create(const DeviceSetup &setup) -> uptr<Device>
{
    uptr<Device> device = nullptr;
    
    switch (setup.mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            device = std::make_unique<gl::SDLDevice>(setup);
            break;
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            device = std::make_unique<vk::SDLDevice>(setup);
            break;
#endif
        case DeviceMode::Null:
            device = std::make_unique<null::Device>(setup);
            break;
        default:
            SL_PANIC("Unknown device mode");
            break;
    }

    if (device)
        device->initSubsystems();
    
    return device;
}

Device::Device(const DeviceSetup &setup):
    setup(setup)
{
}

void Device::initSubsystems()
{
    FriendToken<Device> token;

    logger = std::make_unique<Logger>(token);
    if (!setup.logFilePath.empty())
        logger->setTargetFile(setup.logFilePath);

    renderer = Renderer::create(this, token);
    physics = Physics::create(this, token);
    fs = FileSystem::create(this, token);
    scriptRuntime = ScriptRuntime::create(this, token);
    jobPool = std::make_shared<JobPool>();
}

void Device::cleanupSubsystems()
{
    // Order matters
    jobPool.reset();
    scriptRuntime.reset();
    fs.reset();
    renderer.reset();
    logger.reset();
}

bool Device::isKeyPressed(KeyCode code, bool firstTime) const
{
    const auto where = pressedKeys.find(code);
    return where != pressedKeys.end() && (!firstTime || where->second);
}

bool Device::isKeyReleased(KeyCode code) const
{
    return releasedKeys.find(code) != releasedKeys.end();
}

auto Device::getMouseMotion() const -> Vector2
{
    return {static_cast<float>(mouseDeltaX), static_cast<float>(mouseDeltaY)};
}

bool Device::isMouseButtonDown(MouseButton button, bool firstTime) const
{
    const auto where = pressedMouseButtons.find(button);
    return where != pressedMouseButtons.end() && (!firstTime || where->second);
}

bool Device::isMouseButtonReleased(MouseButton button) const
{
    return releasedMouseButtons.find(button) != releasedMouseButtons.end();
}

void Device::update(std::function<void()> update)
{
    beginUpdate();
    jobPool->update(); // TODO add smth like waitForFinish() to Device and wait in it for background tasks to finish
    update();
    endUpdate();
}

void Device::updateTime()
{
    const auto time = getLifetime();
    timeDelta = time - lastUpdateTime;
    lastUpdateTime = time;
}
