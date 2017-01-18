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

#include "SoloSDLOpenGLDevice.h"

#ifdef SL_OPENGL_RENDERER

#include <vector>
#include <GL/glew.h>
#include <SDL_surface.h>

using namespace solo;


SDLOpenGLDevice::SDLOpenGLDevice(DeviceSetup const &setup):
    SDLDevice(setup)
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

    window = SDL_CreateWindow(setup.windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              setup.canvasWidth, setup.canvasHeight, flags);
    context = SDL_GL_CreateContext(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    SDL_GL_SetSwapInterval(1);
}


SDLOpenGLDevice::~SDLOpenGLDevice()
{
    if (context)
        SDL_GL_DeleteContext(context);
    if (window)
        SDL_DestroyWindow(window);
}


void SDLOpenGLDevice::endUpdate()
{
    SDL_GL_SwapWindow(window);
}


void SDLOpenGLDevice::saveScreenshot(const std::string &path)
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


auto SDLOpenGLDevice::getCanvasSize() const -> Vector2
{
    int32_t width, height;
    SDL_GL_GetDrawableSize(window, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

#endif
