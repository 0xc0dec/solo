/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloOpenGLDevice.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGL.h"

using namespace solo;

OpenGLDevice::OpenGLDevice(DeviceSetup const &setup):
    SDLDevice(setup) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    initWindow(setup.fullScreen, setup.windowTitle.c_str(), setup.canvasWidth, setup.canvasHeight, SDL_WINDOW_OPENGL);

    context_ = SDL_GL_CreateContext(window());
    panicIf(!context_, "Unable to create OpenGL context");

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        cleanup();

    SDL_GL_SetSwapInterval(setup.vsync ? -1 : 0);
}

OpenGLDevice::~OpenGLDevice() {
    shutdownSubsystems();
    cleanup();
}

void OpenGLDevice::cleanup() {
    if (context_) {
        SDL_GL_DeleteContext(context_);
        context_ = nullptr;
    }
}

void OpenGLDevice::endUpdate() {
    SDL_GL_SwapWindow(window());
}

#endif
