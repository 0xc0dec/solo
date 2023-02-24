/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloSDLDebugInterface.h"
#include "SoloSDLDevice.h"
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>

using namespace solo;

SDLDebugInterface::SDLDebugInterface(Device *device):
    DebugInterface(device),
    device_(dynamic_cast<SDLDevice *>(device))
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    device_->onEvent([](auto & evt)
    {
        ImGui_ImplSDL2_ProcessEvent(&evt);
    });
}

SDLDebugInterface::~SDLDebugInterface()
{
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void SDLDebugInterface::beginFrame()
{
    ImGui_ImplSDL2_NewFrame(device_->window());
    ImGui::NewFrame();
}
