/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLSDLDevice.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGL.h"
#include <SDL_surface.h>

using namespace solo;

OpenGLSDLDevice::OpenGLSDLDevice(DeviceSetup const &setup):
    SDLDevice(setup)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    auto flags = static_cast<u32>(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (setup.fullScreen)
        flags |= SDL_WINDOW_FULLSCREEN;

    window_ = SDL_CreateWindow(
        setup.windowTitle.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        setup.canvasWidth, setup.canvasHeight,
        flags
    );
    SL_DEBUG_PANIC(!window_, "Unable to create device window");

    context_ = SDL_GL_CreateContext(window_);
    SL_DEBUG_PANIC(!context_, "Unable to create OpenGL context");

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        cleanup();

    SDL_GL_SetSwapInterval(setup.vsync ? -1 : 0);
}

OpenGLSDLDevice::~OpenGLSDLDevice()
{
    cleanupSubsystems();
    cleanup();
}

void OpenGLSDLDevice::cleanup()
{
    if (context_)
    {
        SDL_GL_DeleteContext(context_);
        context_ = nullptr;
    }
    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
}

void OpenGLSDLDevice::endUpdate()
{
    SDL_GL_SwapWindow(window_);
}

void OpenGLSDLDevice::saveScreenshot(const str &path)
{
    s32 width, height;
    SDL_GetWindowSize(window_, &width, &height);

    auto surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

    // Flip the image
    vec<u8> buf;
    buf.reserve(surface->pitch);
    const auto pixels = static_cast<u8*>(surface->pixels);
    for (s32 row = 0; row < height / 2; ++row)
    {
        const auto row1 = pixels + surface->pitch * row;
        const auto row2 = pixels + surface->pitch * (height - row - 1);
        memcpy(buf.data(), row1, surface->pitch);
        memcpy(row1, row2, surface->pitch);
        memcpy(row2, buf.data(), surface->pitch);
    }

    SDL_SaveBMP(surface, path.c_str());

    SDL_FreeSurface(surface);
}

#endif
