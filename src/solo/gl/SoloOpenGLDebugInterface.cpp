/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloOpenGLDebugInterface.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGLDevice.h"
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl3.h>

using namespace solo;

OpenGLDebugInterface::OpenGLDebugInterface(Device *device):
    SDLDebugInterface(device),
    device_(dynamic_cast<OpenGLDevice *>(device)) {
    ImGui_ImplSDL2_InitForOpenGL(device_->window(), device_->window());
    ImGui_ImplOpenGL3_Init("#version 130"); // GL 3.0 + GLSL 130
}

OpenGLDebugInterface::~OpenGLDebugInterface() {
    ImGui_ImplOpenGL3_Shutdown();
}

void OpenGLDebugInterface::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    SDLDebugInterface::beginFrame();
}

void OpenGLDebugInterface::render() const {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#endif
