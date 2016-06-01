#include "SoloSDLOpenGLDevice.h"
#include "SoloLogger.h"
#include <vector>
#include <GL/glew.h>
#include <SDL_surface.h>

using namespace solo;


SDLOpenGLDevice::SDLOpenGLDevice(DeviceCreationArgs const& args):
    SDLDevice(args)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
        SL_THROW(InternalException, "Failed to initialize device systems");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, creationArgs.depthBits);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    auto flags = static_cast<uint32_t>(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (creationArgs.fullScreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(creationArgs.windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        creationArgs.canvasWidth, creationArgs.canvasHeight, flags);
    if (!window)
        SL_THROW(InternalException, "Failed to create window");

    context = SDL_GL_CreateContext(window);
    if (!context)
        SL_THROW(InternalException, "Failed to create OpengGL context");

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        SL_THROW(InternalException, "Failed to initialize OpenGL extensions");
    }

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    logger->logInfo(SL_FMT("Running in OpenGL ", major, ".", minor, " mode"));

    SDL_GL_SetSwapInterval(1);
}


SDLOpenGLDevice::~SDLOpenGLDevice()
{
    if (context)
        SDL_GL_DeleteContext(context);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}


void SDLOpenGLDevice::endUpdate()
{
    SDL_GL_SwapWindow(window);
}


void SDLOpenGLDevice::saveScreenshot(const std::string& path)
{
    int32_t width, height;
    SDL_GetWindowSize(window, &width, &height);

    auto surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

    // Flip the image
    std::vector<uint8_t> buf;
    buf.reserve(surface->pitch);
    auto pixels = static_cast<uint8_t*>(surface->pixels);
    for (int32_t row = 0; row < height / 2; ++row)
    {
        auto row1 = pixels + surface->pitch * row;
        auto row2 = pixels + surface->pitch * (height - row - 1);
        memcpy(buf.data(), row1, surface->pitch);
        memcpy(row1, row2, surface->pitch);
        memcpy(row2, buf.data(), surface->pitch);
    }

    SDL_SaveBMP(surface, path.c_str());

    SDL_FreeSurface(surface);
}


auto SDLOpenGLDevice::getCanvasSize() const -> Vector2
{
    int32_t width, height;
    SDL_GL_GetDrawableSize(window, &width, &height);
    return Vector2(static_cast<float>(width), static_cast<float>(height));
}
