/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloRenderer.h"
#include "SoloPhysics.h"
#include "SoloScriptRuntime.h"
#include "SoloJobPool.h"
#include "SoloEnums.h"
#include "SoloDebugInterface.h"
#include "gl/SoloOpenGLDevice.h"
#include "vk/SoloVulkanDevice.h"

using namespace solo;

auto Device::create(const DeviceSetup &setup) -> uptr<Device>
{
    uptr<Device> device;

    switch (setup.mode)
    {
#ifdef SL_OPENGL_RENDERER
    case DeviceMode::OpenGL:
        device = std::make_unique<OpenGLDevice>(setup);
        break;
#endif
#ifdef SL_VULKAN_RENDERER
    case DeviceMode::Vulkan:
        device = std::make_unique<VulkanDevice>(setup);
        break;
#endif
    default:
        panic("Unknown device mode");
        break;
    }

    if (device)
        device->initSubsystems(setup);

    return device;
}

Device::Device(const DeviceSetup &setup):
    mode_(setup.mode),
    vsync_(setup.vsync)
{
}

void Device::initSubsystems(const DeviceSetup &setup)
{
    if (!setup.logFilePath.empty())
        Logger::global().setOutputFile(setup.logFilePath);

    jobPool_ = std::make_shared<JobPool>();
    physics_ = Physics::fromDevice(this);
    fs_ = FileSystem::fromDevice(this);
    scriptRuntime_ = ScriptRuntime::fromDevice(this);
    renderer_ = Renderer::fromDevice(this);
    debugInterface_ = DebugInterface::fromDevice(this);

    renderer_->bootstrap();
}

void Device::shutdownSubsystems()
{
    renderer_->cleanup();

    // Order matters
    debugInterface_.reset();
    jobPool_.reset();
    scriptRuntime_.reset();
    physics_.reset();
    fs_.reset();
    renderer_.reset();
}

bool Device::hasActiveBackgroundJobs() const
{
    return jobPool_->hasActiveJobs();
}

bool Device::isKeyPressed(KeyCode code, bool firstTime) const
{
    const auto where = pressedKeys_.find(code);
    return where != pressedKeys_.end() && (!firstTime || where->second);
}

bool Device::isKeyReleased(KeyCode code) const
{
    return releasedKeys_.find(code) != releasedKeys_.end();
}

bool Device::isMouseButtonDown(MouseButton button, bool firstTime) const
{
    const auto where = pressedMouseButtons_.find(button);
    return where != pressedMouseButtons_.end() && (!firstTime || where->second);
}

bool Device::isMouseButtonReleased(MouseButton button) const
{
    return releasedMouseButtons_.find(button) != releasedMouseButtons_.end();
}

void Device::update(const std::function<void()> &update)
{
    beginUpdate();
    jobPool_->update(); // TODO add smth like waitForFinish() to Device and wait in it for background tasks to finish
    physics_->update();
    renderer_->renderFrame([&]()
    {
        debugInterface_->renderFrame(update);
    });
    endUpdate();
}

void Device::updateTime()
{
    const auto time = lifetime();
    timeDelta_ = time - lastUpdateTime_;
    lastUpdateTime_ = time;
}
