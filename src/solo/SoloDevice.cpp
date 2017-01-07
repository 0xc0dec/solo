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
#include "platform/vulkan/SoloSDLVulkanDevice.h"

using namespace solo;


auto Device::create(const DeviceSetup &setup) -> uptr<Device>
{
    uptr<Device> device = nullptr;
    
    switch (setup.mode)
    {
        case DeviceMode::OpenGL:
            device = std::make_unique<SDLOpenGLDevice>(setup);
            break;
        case DeviceMode::Vulkan:
            device = std::make_unique<SDLVulkanDevice>(setup);
            break;
        case DeviceMode::Null:
            device = std::make_unique<NullDevice>(setup);
            break;
        default:
            SL_ERR("Unknown device mode");
            break;
    }

    if (device)
        device->initSubsystems();
    
    return device;
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
    assetLoader = std::make_unique<AssetLoader>(this, token);
    graphics = Graphics::create(this, token);
    scene = std::make_unique<Scene>(this, token);
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
