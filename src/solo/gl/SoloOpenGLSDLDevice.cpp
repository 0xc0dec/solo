/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloOpenGLSDLDevice.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGL.h"

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
}

void OpenGLSDLDevice::endUpdate()
{
    SDL_GL_SwapWindow(window_);
}

#endif
