/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSDLOpenGLDevice.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGL.h"
#include <vector>
#include <SDL_surface.h>

using namespace solo;
using namespace gl;

SDLDevice::SDLDevice(DeviceSetup const &setup):
    Device(setup)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, setup.redBits);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, setup.greenBits);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, setup.blueBits);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, setup.alphaBits);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, setup.depthBits);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    auto flags = static_cast<uint32_t>(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (setup.fullScreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(setup.windowTitle.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        setup.canvasWidth, setup.canvasHeight, flags);
    context = SDL_GL_CreateContext(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        cleanup();

    SDL_GL_SetSwapInterval(1);
}

SDLDevice::~SDLDevice()
{
    cleanupSubsystems();
    cleanup();
}

void SDLDevice::cleanup()
{
    if (context)
    {
        SDL_GL_DeleteContext(context);
        context = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void SDLDevice::endUpdate()
{
    SDL_GL_SwapWindow(window);
}

void SDLDevice::saveScreenshot(const str &path)
{
    int32_t width, height;
    SDL_GetWindowSize(window, &width, &height);

    auto surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

    // Flip the image
    std::vector<uint8_t> buf;
    buf.reserve(surface->pitch);
    auto pixels = static_cast<uint8_t *>(surface->pixels);
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

#endif
