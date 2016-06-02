#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloAssetLoader.h"
#include "SoloScene.h"
#include "SoloLogger.h"
#include "SoloRenderer.h"
#include "SoloGraphics.h"
#include "SoloPhysics.h"
#include "SoloEffectRepository.h"
#include "platform/stub/SoloStubDevice.h"
#include "platform/opengl/SoloSDLOpenGLDevice.h"
#include "platform/vulkan/SoloSDLVulkanDevice.h"

using namespace solo;


// The whole system is made as singleton for the sake of coding convenience.
// For instance, to make resource creation look like Resource::create(args)
// instead of device->getResourceManager()->createResource(args)
uptr<Device> Device::instance = nullptr;


auto DeviceCreationArgs::withMode(DeviceMode mode) -> DeviceCreationArgs&
{
    this->mode = mode;
    return *this;
}


auto DeviceCreationArgs::withDimensions(uint32_t canvasWidth, uint32_t canvasHeight) -> DeviceCreationArgs&
{
    this->canvasWidth = canvasWidth;
    this->canvasHeight = canvasHeight;
    return *this;
}


auto DeviceCreationArgs::withFullScreen(bool fullScreen) -> DeviceCreationArgs&
{
    this->fullScreen = fullScreen;
    return *this;
}


auto DeviceCreationArgs::withWindowTitle(const std::string& title) -> DeviceCreationArgs&
{
    windowTitle = title;
    return *this;
}


auto DeviceCreationArgs::withColorBits(uint32_t redBits, uint32_t greenBits, uint32_t blueBits, uint32_t alphaBits) -> DeviceCreationArgs&
{
    this->redBits = redBits;
    this->greenBits = greenBits;
    this->blueBits = blueBits;
    this->alphaBits = alphaBits;
    return *this;
}


auto DeviceCreationArgs::withDepthBits(uint32_t depthBits) -> DeviceCreationArgs&
{
    this->depthBits = depthBits;
    return *this;
}


auto DeviceCreationArgs::withLogPath(const std::string& logPath) -> DeviceCreationArgs&
{
    this->logFilePath = logPath;
    return *this;
}


auto Device::init(const DeviceCreationArgs& args) -> Device*
{
    if (!instance)
    {
        switch (args.mode)
        {
            case DeviceMode::OpenGL:
                instance = std::make_unique<SDLOpenGLDevice>(args);
                break;
#if 0
            case DeviceMode::Vulkan:
                instance = std::make_unique<SDLVulkanDevice>(args);
                break;
#endif
            default:
                instance = std::make_unique<StubDevice>(args);
                break;
        }
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

    if (effectRepository)
        effectRepository.reset();

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
    effectRepository = EffectRepository::create(token);
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
