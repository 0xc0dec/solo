#include "SoloSDLDevice.h"

using namespace solo;


SDLDevice::SDLDevice(const DeviceCreationArgs& args)
{
}


auto SDLDevice::getWindowTitle() const -> std::string
{
    return std::string(SDL_GetWindowTitle(window));
}


void SDLDevice::setWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(window, title.c_str());
}


void SDLDevice::setCursorCaptured(bool captured)
{
    SDL_SetRelativeMouseMode(captured ? SDL_TRUE : SDL_FALSE);
}
