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
#include "gl/SoloOpenGLSDLDevice.h"
#include "vk/SoloVulkanSDLDevice.h"

using namespace solo;

auto Device::create(const DeviceSetup &setup) -> uptr<Device>
{
    uptr<Device> device = nullptr;
    
    switch (setup.mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            device = std::make_unique<OpenGLSDLDevice>(setup);
            break;
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            device = std::make_unique<VulkanSDLDevice>(setup);
            break;
#endif
        default:
            SL_DEBUG_PANIC(true, "Unknown device mode");
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
    const FriendToken<Device> token;

    if (!setup.logFilePath.empty())
        Logger::global().setOutputFile(setup.logFilePath);

    renderer_ = Renderer::create(this, token);
    physics_ = Physics::create(this, token);
    fs_ = FileSystem::create(this, token);
    scriptRuntime_ = ScriptRuntime::withDevice(this, token);
    jobPool_ = std::make_shared<JobPool>();
}

void Device::cleanupSubsystems()
{
    // Order matters
    jobPool_.reset();
    scriptRuntime_.reset();
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
    renderer_->renderFrame([&]() { update(); });
    endUpdate();
}

void Device::updateTime()
{
    const auto time = lifetime();
    timeDelta_ = time - lastUpdateTime_;
    lastUpdateTime_ = time;
}
